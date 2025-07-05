#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include "enemy.h"
#include "ship.h"
#include "config.h"
#include "tools.h"
#include "camera.h"
#include "renderer.h"


static Uint32 lastEnemyGenerationTime = 0;

static Uint32 lastLaserGenerationTime = 0;

static Laser *lasers = NULL;
static int lasersCount = 0;


void generateEnemy(Ship targetShip, Ship **ships, int *shipCount) {
    Ship enemyShip;

    enemyShip.shiptype = ENEMY;
    enemyShip.id = *shipCount;
    enemyShip.idModel = 6;
    enemyShip.state = ATTACKING_SHIP;
    
    enemyShip.base.type = SPOT_NONE;
    enemyShip.target.type = SPOT_SHIP;
    enemyShip.target.id_ship = targetShip.id;

    enemyShip.x = targetShip.x + rand() % 5000 - 2500;
    enemyShip.y = targetShip.y + rand() % 5000 - 2500;
    enemyShip.w = 200;
    enemyShip.h = 200;
    enemyShip.speed = (rand() / (float)RAND_MAX * 0.6 + 0.4) * SHIP_SPEED;
    enemyShip.level = 1;
    
    enemyShip.maxLife = 100;
    enemyShip.currentLife = 100;
    enemyShip.fuelConsumption = 1;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
    enemyShip.range = 1000;

    enemyShip.waitStartTime = 0;
    enemyShip.frameIndex = rand() % 4;  // Desynchronisation des fusees
    enemyShip.lastFrameTime = 0;
    enemyShip.lastRefreshFilling = SDL_GetTicks();

    addShip(enemyShip, ships, shipCount);
}

void updateWarSystem(Ship **ships, int *shipCount, Mix_Chunk **sounds) {
    updateEnemies(ships, shipCount);
    updateLasers(*ships, sounds);
    newLasersFired(*ships, shipCount, sounds);
    deleteKilledShips(ships, shipCount);
}

void updateEnemies(Ship **ships, int *shipCount) {
    if (SDL_GetTicks() - lastEnemyGenerationTime < ENEMY_GENERATION_PERIOD) return;

    lastEnemyGenerationTime = SDL_GetTicks();
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].shiptype != ENEMY) {
            int newEnemiesCount = rand() % (*ships)[i].noise;
            for (int j = 0; j < newEnemiesCount; j++) {  // Le nombre de nouveaux ennemies depend de la variable 'noise'
                generateEnemy((*ships)[i], ships, shipCount);
            }
        }
    }
}

void updateLasers(Ship *ships, Mix_Chunk **sounds) {
    float distance;
    SDL_Rect rectShip;

    for (int i = 0; i < lasersCount; i++) {
        rectShip = (SDL_Rect){ships[lasers[i].target_id].x, ships[lasers[i].target_id].y, ships[lasers[i].target_id].w, ships[lasers[i].target_id].h};
        distance = dist(lasers[i].target_coords.x, 
                        lasers[i].target_coords.y,
                        lasers[i].rect.x,
                        lasers[i].rect.y);

        if (SDL_HasIntersection(&lasers[i].rect, &rectShip)) {
            // Appliquer des dégâts
            if (lasers[i].target_id != -1) {  // Vaut -1 lorsque la cible du laser a deja ete detruite
                ships[lasers[i].target_id].currentLife -= DAMAGE;
                Mix_PlayChannel(3, sounds[12], 0); // Explosion
            }

            // Supprimer le laser
            deleteLaser(i);
        } else if (distance > 10.f) {
            // Avancer le laser vers sa cible initiale
            lasers[i].rect.x += (lasers[i].target_coords.x - lasers[i].rect.x) * lasers[i].speed / distance;
            lasers[i].rect.y += (lasers[i].target_coords.y - lasers[i].rect.y) * lasers[i].speed / distance;
        } else {
            deleteLaser(i);
        }
    }
}

void newLasersFired(Ship *ships, int *shipCount, Mix_Chunk **sounds) {
    if (SDL_GetTicks() - lastLaserGenerationTime < LASER_GENERATION_PERIOD) return;
        

    lastLaserGenerationTime = SDL_GetTicks();

    SDL_Rect newLaser = {0, 0, 200, 200};
    for (int i = 0; i < *shipCount; i++) {
        if (ships[i].shiptype != ENEMY) continue;  // Seuls les ennemies peuvent tirer des missiles actuellement

        if (ships[i].target.id_ship == -1) continue;

        // Initialise les coordonnees du nouveau laser
        newLaser.x = ships[i].x;
        newLaser.y = ships[i].y;

        // Ajout du nouveau laser + bruitage
        addLaser(newLaser,
                 (SDL_Point){ships[ships[i].target.id_ship].x, ships[ships[i].target.id_ship].y},
                 ships[i].target.id_ship, 
                 computeAngleDeg(ships[ships[i].target.id_ship].x, ships[ships[i].target.id_ship].y, newLaser.x, newLaser.y),
                 10);

        Mix_PlayChannel(2, sounds[11], 0);
    }
}

void addLaser(SDL_Rect rect, SDL_Point target_coords, int target_id, float angle, int speed) {
    Laser *temp = realloc(lasers, (lasersCount + 1) * sizeof(Laser));
    if (temp != NULL) {
        lasers = temp;
        lasers[lasersCount] = (Laser){rect, target_coords, angle, speed, target_id};
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
    lasers = realloc(lasers, lasersCount * sizeof(Laser));
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
    for (int i = 0; i<lasersCount; i++) {
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
