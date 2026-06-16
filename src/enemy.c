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
#include "threat.h"


static Uint32 lastEnemyGenerationTime = 0;
static Uint32 lastTowerFireTime = 0;

static Laser *lasers = NULL;
static int lasersCount = 0;

static int lastLaserSon = 2;  // canaux 2,3,4 pour les sons de lasers
static int lastExplosionSon = 5;   /// canaux 5,6,7 pour les cons d'explosions


void generateEnemy(int targetIndex, int raidPlanet, int boss, Ship **ships, int *shipCount, Planet *planets) {
    // Position d'apparition (autour de la cible) avant la reallocation
    float anchorX, anchorY;
    if (raidPlanet >= 0) {
        anchorX = planets[raidPlanet].x;
        anchorY = planets[raidPlanet].y;
    } else {
        anchorX = (*ships)[targetIndex].x;
        anchorY = (*ships)[targetIndex].y;
    }

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
    float spawnDist = 1800 + rand() % 1500;
    enemy->w = 160;
    enemy->h = 160;
    enemy->x = anchorX + spawnDist * cosf(angle) - enemy->w / 2.f;
    enemy->y = anchorY + spawnDist * sinf(angle) - enemy->h / 2.f;

    enemy->speed = (0.5f + (rand() % 50) / 100.f) * SHIP_SPEED * (1.0f + 0.03f * threatLevel());
    enemy->maxLife = (int)(ENEMY_LIFE * threatFactor());  // Pirates plus resistants quand la menace monte
    enemy->currentLife = enemy->maxLife;
    enemy->range = 1300;
    enemy->noise = 0;
    enemy->fuelConsumption = 1;  // Valeur non nulle par securite (evite des divisions par zero)

    enemy->state = ATTACKING_SHIP;
    enemy->base.type = SPOT_NONE;
    if (raidPlanet >= 0) {
        // Pillard : vise une planete colonisee pour voler ses minerais
        enemy->maxLife = (int)(enemy->maxLife * 1.3f);  // Plus coriace
        enemy->currentLife = enemy->maxLife;
        enemy->target.type = SPOT_PLANET;
        enemy->target.id_planet = raidPlanet;
    } else {
        enemy->target.type = SPOT_SHIP;
        enemy->target.id_ship = targetIndex;
    }

    if (boss) {
        // Cuirasse pirate : enorme, lent, tres resistant, prime massive
        enemy->maxLife *= 8;
        enemy->currentLife = enemy->maxLife;
        enemy->w = 340;
        enemy->h = 340;
        enemy->speed *= 0.55f;
        enemy->range += 600;
    }

    enemy->frameIndex = rand() % 4;
    enemy->lastFrameTime = 0;
    enemy->lastRefreshFiring = SDL_GetTicks();

    (*shipCount)++;
}

static int isColonized(Planet *p) {
    if (p->planetType == SUN) return 0;
    for (int b = 0; b < BUILD_TYPE_COUNT; b++) {
        if (p->builds[b].level > 0) return 1;
    }
    return 0;
}

void updateWarSystem(Ship **ships, int *shipCount, Planet *planets, int planetCount, Mix_Chunk **sounds) {
    updateEnemies(ships, shipCount, planets, planetCount);
    updateDefenceTowers(planets, planetCount, *ships, *shipCount, sounds);
    updateDefenders(*ships, *shipCount, sounds);
    updateRaiders(*ships, *shipCount, planets, planetCount);
    updateLasers(*ships, sounds);
    newLasersFired(*ships, *shipCount, sounds);
    deleteKilledShips(ships, shipCount);
}

void updateEnemies(Ship **ships, int *shipCount, Planet *planets, int planetCount) {
    // Periode de grace au demarrage (laisse le temps au joueur de s'installer)
    if (lastEnemyGenerationTime == 0) {
        lastEnemyGenerationTime = SDL_GetTicks();
        return;
    }

    // La cadence d'apparition accelere avec la menace (et double pendant une deferlante)
    Uint32 period = (Uint32)(ENEMY_GENERATION_PERIOD / (threatFactor() * (eventPirateSurge() ? 2.0f : 1.0f)));
    if (period < 1500) period = 1500;
    if (SDL_GetTicks() - lastEnemyGenerationTime < period) return;
    lastEnemyGenerationTime = SDL_GetTicks();

    // Comptage des ennemis et des transporteurs
    int enemies = 0, transporters = 0;
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].shiptype == ENEMY) enemies++;
        else transporters++;
    }

    // Le nombre de pirates simultanes augmente avec la flotte et la menace
    int cap = 2 + 2 * transporters + threatLevel();
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

    if (targetIndex == -1) return;

    // A partir du niveau de menace 2, certains pirates deviennent des pillards
    // qui s'attaquent directement a vos colonies (probabilite croissante).
    int raidPlanet = -1;
    if (threatLevel() >= 2 && (rand() % 100) < (20 + 5 * threatLevel())) {
        int colonizedList[256];
        int n = 0;
        for (int i = 0; i < planetCount && n < 256; i++) {
            if (isColonized(&planets[i])) colonizedList[n++] = i;
        }
        if (n > 0) raidPlanet = colonizedList[rand() % n];
    }

    // Cuirasses pirates (boss) a partir du niveau de menace 4
    int boss = (threatLevel() >= 4 && (rand() % 100) < (2 + threatLevel()));

    generateEnemy(targetIndex, raidPlanet, boss, ships, shipCount, planets);
    if (boss) {
        pushNotification("WARNING: a pirate dreadnought has arrived!", (SDL_Color){255, 140, 0, 255});
    } else if (raidPlanet >= 0) {
        pushNotification("Raiders are heading for one of your colonies!", RED);
    } else {
        pushNotification("Pirate detected near your fleet!", RED);
    }
}

void updateRaiders(Ship *ships, int shipCount, Planet *planets, int planetCount) {
    static Uint32 lastRaidNotif = 0;
    Uint32 now = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        if (ships[i].shiptype != ENEMY) continue;
        if (ships[i].target.type != SPOT_PLANET) continue;
        int pid = ships[i].target.id_planet;
        if (pid < 0 || pid >= planetCount) continue;

        float d = distanceShipPlanet(&ships[i], &planets[pid]);
        if (d > RAID_RANGE) continue;  // Pas encore arrive

        if (now - ships[i].lastRefreshFiring < RAID_PERIOD) continue;
        ships[i].lastRefreshFiring = now;

        // Vol de minerais dans chaque reservoir construit de la planete
        int stolen = 0;
        for (int b = 2; b < BUILD_TYPE_COUNT; b += 2) {
            Build *store = &planets[pid].builds[b];
            if (store->type == ORE_STORE && store->level > 0 && store->tank.currentCapacity > 0) {
                int take = (int)(RAID_STEAL * raidStealMultiplier());  // Boucliers planetaires
                if (take < 1) take = 1;
                if (take > store->tank.currentCapacity) take = store->tank.currentCapacity;
                store->tank.currentCapacity -= take;
                stolen += take;
            }
        }
        ships[i].transferredMinerals += stolen;

        // Occasionnellement, les pillards endommagent un batiment (perte de niveau)
        // Les boucliers planetaires reduisent cette probabilite
        if (rand() % 100 < (int)(33 * raidStealMultiplier())) {
            for (int tries = 0; tries < 6; tries++) {
                int b = 2 + rand() % (BUILD_TYPE_COUNT - 2);
                Build *bd = &planets[pid].builds[b];
                if (bd->level > 0) {
                    bd->level--;
                    if (bd->type == ORE_MINE && bd->mine.productivity > 200) bd->mine.productivity *= 0.9;
                    break;
                }
            }
        }

        if (now - lastRaidNotif > 4000) {
            lastRaidNotif = now;
            pushNotification("A colony is being raided! Defend it!", RED);
        }
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

void updateDefenders(Ship *ships, int shipCount, Mix_Chunk **sounds) {
    Uint32 now = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        if (ships[i].shiptype != DEFENDER) continue;

        // Recherche du pirate le plus proche dans la portee du defenseur
        int best = -1;
        float bestDist = ships[i].range;
        for (int s = 0; s < shipCount; s++) {
            if (ships[s].shiptype != ENEMY) continue;
            float d = distanceShipShip(&ships[i], &ships[s]);
            if (d < bestDist) {
                bestDist = d;
                best = s;
            }
        }

        if (best == -1) {
            // Aucun ennemi : le defenseur se met en garde (immobile)
            if (ships[i].state == ATTACKING_SHIP) ships[i].state = WAITING_ON_BASE;
            continue;
        }

        // Poursuite de la cible
        ships[i].state = ATTACKING_SHIP;
        ships[i].target.type = SPOT_SHIP;
        ships[i].target.id_ship = best;

        // Tir si le pirate est a portee de feu et que la cadence le permet
        if (now - ships[i].lastRefreshFiring < LASER_GENERATION_PERIOD - 100) continue;
        ships[i].lastRefreshFiring = now;

        SDL_Rect laser = {(int)(ships[i].x + ships[i].w / 2), (int)(ships[i].y + ships[i].h / 2), 200, 200};
        addLaser(laser,
                 (SDL_Point){(int)ships[best].x, (int)ships[best].y},
                 best,
                 computeAngleDeg((int)ships[best].x, (int)ships[best].y, laser.x, laser.y),
                 16,
                 2 + towerDamageBonus());  // Defenseurs : degats solides

        Mix_PlayChannel(lastLaserSon, sounds[11], 0);
        Mix_SetPositionCameraCentered(lastLaserSon, &lasers[lasersCount - 1].rect);
        lastLaserSon++;
        if (lastLaserSon == 5) lastLaserSon = 2;
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
                 enemyDamage());

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
