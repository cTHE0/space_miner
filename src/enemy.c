#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <math.h>
#include "enemy.h"
#include "ship.h"
#include "planet.h"
#include "build.h"
#include "config.h"
#include "tools.h"
#include "camera.h"
#include "renderer.h"
#include "notify.h"
#include "assets_gestion.h"
#include "meta.h"
#include "events.h"


static Uint32 lastEnemyGenerationTime = 0;
static Uint32 lastTowerFireTime = 0;

static Laser *lasers = NULL;
static int lasersCount = 0;

static int lastLaserSon = 2;  // canaux 2,3,4 pour les sons de lasers
static int lastExplosionSon = 5;   /// canaux 5,6,7 pour les cons d'explosions


void generateEnemy(int targetIndex, Ship **ships, int *shipCount) {
    // Position de la cible avant la reallocation (qui invalide les pointeurs)
    float targetX = (*ships)[targetIndex].x;
    float targetY = (*ships)[targetIndex].y;

    Ship *temp = realloc(*ships, (*shipCount + 1) * sizeof(Ship));
    if (temp == NULL) {
        return;  // Echec d'allocation : on n'ajoute pas d'ennemi
    }
    *ships = temp;

    Ship *enemy = &(*ships)[*shipCount];
    memset(enemy, 0, sizeof(Ship));

    enemy->shiptype = ENEMY;
    enemy->id = *shipCount;
    enemy->idModel = 0;
    enemy->level = 1;

    // Apparition autour de la cible, juste hors de portee
    float angle = (rand() % 360) * M_PI / 180.0f;
    float spawnDist = 1500 + rand() % 1200;
    enemy->w = 160;
    enemy->h = 160;
    enemy->x = targetX + spawnDist * cosf(angle) - enemy->w / 2.f;
    enemy->y = targetY + spawnDist * sinf(angle) - enemy->h / 2.f;

    enemy->speed = (0.5f + (rand() % 50) / 100.f) * SHIP_SPEED;
    enemy->maxLife = ENEMY_LIFE;
    enemy->currentLife = ENEMY_LIFE;
    enemy->range = 1300;
    enemy->noise = 0;
    enemy->fuelConsumption = 1;  // Valeur non nulle par securite (evite des divisions par zero)

    enemy->state = ATTACKING_SHIP;
    enemy->base.type = SPOT_NONE;
    enemy->target.type = SPOT_SHIP;
    enemy->target.id_ship = targetIndex;

    enemy->frameIndex = rand() % 4;
    enemy->lastFrameTime = 0;
    enemy->lastRefreshFiring = SDL_GetTicks();

    (*shipCount)++;
}

void updateWarSystem(Ship **ships, int *shipCount, Planet *planets, int planetCount, Mix_Chunk **sounds) {
    updateEnemies(ships, shipCount);
    updateDefenceTowers(planets, planetCount, *ships, *shipCount, sounds);
    updateLasers(*ships, sounds);
    newLasersFired(*ships, *shipCount, sounds);
    deleteKilledShips(ships, shipCount);
}

void updateEnemies(Ship **ships, int *shipCount) {
    // Periode de grace au demarrage (laisse le temps au joueur de s'installer)
    if (lastEnemyGenerationTime == 0) {
        lastEnemyGenerationTime = SDL_GetTicks();
        return;
    }

    // La deferlante de pirates double la cadence d'apparition
    Uint32 period = eventPirateSurge() ? ENEMY_GENERATION_PERIOD / 2 : ENEMY_GENERATION_PERIOD;
    if (SDL_GetTicks() - lastEnemyGenerationTime < period) return;
    lastEnemyGenerationTime = SDL_GetTicks();

    // Comptage des ennemis et des transporteurs
    int enemies = 0, transporters = 0;
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].shiptype == ENEMY) enemies++;
        else transporters++;
    }

    // Le nombre de pirates simultanes augmente avec la taille de la flotte du joueur
    int cap = 2 + 2 * transporters;
    if (eventPirateSurge()) cap += 4;  // Plus de pirates pendant une deferlante
    if (cap > MAX_ENEMIES) cap = MAX_ENEMIES;

    if (transporters == 0 || enemies >= cap) return;

    // Choix d'un transporteur cible au hasard (echantillonnage par reservoir)
    int targetIndex = -1, count = 0;
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].shiptype != ENEMY) {
            count++;
            if (rand() % count == 0) targetIndex = i;
        }
    }

    if (targetIndex != -1) {
        generateEnemy(targetIndex, ships, shipCount);
        pushNotification("Pirate detected near your fleet!", RED);
    }
}

void updateDefenceTowers(Planet *planets, int planetCount, Ship *ships, int shipCount, Mix_Chunk **sounds) {
    if (SDL_GetTicks() < lastTowerFireTime + TOWER_FIRE_PERIOD) return;
    lastTowerFireTime = SDL_GetTicks();

    for (int p = 0; p < planetCount; p++) {
        Build *tower = &planets[p].builds[1];
        if (tower->type != DEFENCE_TOWER || tower->level == 0) continue;

        float range = (4000 + tower->level * 1500.f) * towerRangeMultiplier();  // Portee (niveau tour + ameliorations globales)

        // Recherche de l'ennemi le plus proche dans la portee
        int best = -1;
        float bestDist = range;
        for (int s = 0; s < shipCount; s++) {
            if (ships[s].shiptype != ENEMY) continue;
            float dx = ships[s].x + ships[s].w / 2.f - planets[p].x;
            float dy = ships[s].y + ships[s].h / 2.f - planets[p].y;
            float d = sqrtf(dx * dx + dy * dy);
            if (d < bestDist) {
                bestDist = d;
                best = s;
            }
        }

        if (best != -1) {
            SDL_Rect laser = {(int)planets[p].x, (int)planets[p].y, 200, 200};
            addLaser(laser,
                     (SDL_Point){(int)ships[best].x, (int)ships[best].y},
                     best,
                     computeAngleDeg((int)ships[best].x, (int)ships[best].y, laser.x, laser.y),
                     16,
                     DAMAGE + towerDamageBonus());  // Les tours frappent plus fort avec les ameliorations

            Mix_PlayChannel(lastLaserSon, sounds[11], 0);
            Mix_SetPositionCameraCentered(lastLaserSon, &lasers[lasersCount - 1].rect);
            lastLaserSon++;
            if (lastLaserSon == 5) lastLaserSon = 2;
        }
    }
}

void updateLasers(Ship *ships, Mix_Chunk **sounds) {
    float distance;
    SDL_Rect targetLaserRect;

    for (int i = 0; i < lasersCount; i++) {
        if (lasers[i].target_id == -1) {
            targetLaserRect = (SDL_Rect){0, 0, -1, -1};  // Valeurs bizarres exprès
        } else {
            targetLaserRect = (SDL_Rect){ships[lasers[i].target_id].x, ships[lasers[i].target_id].y, ships[lasers[i].target_id].w, ships[lasers[i].target_id].h};
        }

        distance = dist(lasers[i].target_coords.x, 
                        lasers[i].target_coords.y,
                        lasers[i].rect.x,
                        lasers[i].rect.y); 

        if (targetLaserRect.w != -1 && SDL_HasIntersection(&lasers[i].rect, &targetLaserRect)) {
            // Appliquer des dégâts
            if (lasers[i].target_id != -1) {  // Vaut -1 lorsque la cible du laser a deja ete detruite
                ships[lasers[i].target_id].currentLife -= lasers[i].damage;
                Mix_PlayChannel(lastExplosionSon, sounds[12], 0);  // Explosion
                Mix_SetPositionCameraCentered(lastExplosionSon, &lasers[i].rect);  // Ne fonctionne que si le canal est actif
                lastExplosionSon ++;
                if (lastExplosionSon == 8) lastExplosionSon = 5;
            }

            // Supprimer le laser
            deleteLaser(i);
        } else if (distance >= lasers[i].speed) {
            // Avancer le laser vers sa cible initiale
            lasers[i].rect.x += (lasers[i].target_coords.x - lasers[i].rect.x) * lasers[i].speed / distance;
            lasers[i].rect.y += (lasers[i].target_coords.y - lasers[i].rect.y) * lasers[i].speed / distance;
        } else {
            deleteLaser(i);
        }
    }
}

void newLasersFired(Ship *ships, int shipCount, Mix_Chunk **sounds) {
    Uint32 currentTime = SDL_GetTicks();
    SDL_Rect newLaser = {0, 0, 200, 200};
    for (int i = 0; i < shipCount; i++) {
        if (ships[i].shiptype != ENEMY) continue;  // Seuls les ennemies peuvent tirer des missiles actuellement
        if (ships[i].target.type != SPOT_SHIP) continue;
        if (ships[i].target.id_ship < 0 || ships[i].target.id_ship >= shipCount) continue;
        if (currentTime < ships[i].lastRefreshFiring + LASER_GENERATION_PERIOD) continue;

        // Met a jour la date du dernier tir
        ships[i].lastRefreshFiring = currentTime;

        // Initialise les coordonnees du nouveau laser
        newLaser.x = ships[i].x;
        newLaser.y = ships[i].y;

        // Ajout du nouveau laser + bruitage
        addLaser(newLaser,
                 (SDL_Point){ships[ships[i].target.id_ship].x, ships[ships[i].target.id_ship].y},
                 ships[i].target.id_ship,
                 computeAngleDeg(ships[ships[i].target.id_ship].x, ships[ships[i].target.id_ship].y, newLaser.x, newLaser.y),
                 10,
                 DAMAGE);

        Mix_PlayChannel(lastLaserSon, sounds[11], 0);
        Mix_SetPositionCameraCentered(lastLaserSon, &lasers[lasersCount - 1].rect);  // Ne fonctionne que si le canal est actif
        lastLaserSon ++;
        if (lastLaserSon == 5) lastLaserSon = 2;
    }
}

void addLaser(SDL_Rect rect, SDL_Point target_coords, int target_id, float angle, int speed, int damage) {
    Laser *temp = realloc(lasers, (lasersCount + 1) * sizeof(Laser));
    if (temp != NULL) {
        lasers = temp;
        lasers[lasersCount] = (Laser){rect, target_coords, angle, speed, target_id, damage};
        lasersCount++;
    } else {
        printf("ERREUR reallocation mémoire laser\n");
    }
}

void deleteLaser(int id) {
    // Remplissage du trou dans le tableau, cree par la suppression de lasers[id]
    lasers[id] = lasers[lasersCount - 1];

    // Suppression de l'emplacement en trop, d'indice 'laserCount - 1'
    lasersCount --;
    if (lasersCount != 0) {
        lasers = realloc(lasers, lasersCount * sizeof(Laser));
    }
}

int newTarget(Ship *ships, int shipCount, int idEnemy, int idException) {
    for (int i = 0; i < shipCount; i++) {
        if (ships[i].shiptype != ENEMY && i != idException && distanceShipShip(&ships[i], &ships[idEnemy]) < ships[idEnemy].range) {
            return i;
        }
    }

    return -1;    // Si aucun vaisseau allié ne se trouve dans la range de l'ennemi, aucune cible à attaquer
}

void displayLasers(SDL_Texture ***imageTextures) {
    SDL_Rect dstRect;
    for (int i = 0; i < lasersCount; i++) {
        dstRect.x = (lasers[i].rect.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
        dstRect.y = (lasers[i].rect.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
        dstRect.w = lasers[i].rect.w * getCameraScale();
        dstRect.h = lasers[i].rect.h * getCameraScale();
        SDL_RenderCopyEx(renderer, imageTextures[5][10], NULL, &dstRect, lasers[i].angle, NULL, SDL_FLIP_NONE);
    }
}

int getLasersCount(void) {
    return lasersCount;
}

Laser *getLasersList(void) {
    return lasers;
}
