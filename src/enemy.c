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

Laser *lasers = NULL;
int lasersCount = 0;

void addLaser(SDL_Rect rect, SDL_Point target_coords, float angle, int speed) {
    Laser *temp = realloc(lasers, (lasersCount + 1) * sizeof(Laser));
    if (temp != NULL) {
        lasers = temp;
        lasers[lasersCount] = (Laser){rect, target_coords, angle, speed, lasersCount};
        lasersCount++;
    } else {
        printf("ERREUR reallocation mémoire laser\n");
    }
}

void deleteLaser(int i) {
    printf("DELETE\n");
    if (i < 0 || i >= lasersCount) {
        printf("Indice de laser invalide: %d\n", i);
        return;
    }

    // Décalage vers la gauche pour supprimer le laser à l'indice i
    for (int j = i; j < lasersCount - 1; j++) {
        lasers[j] = lasers[j + 1];
        lasers[j].i = j; // Met à jour l'indice interne du laser
    }

    lasersCount--;

    // Réallocation de la mémoire pour réduire la taille
    if (lasersCount > 0) {
        Laser *temp = realloc(lasers, lasersCount * sizeof(Laser));
        if (temp != NULL) {
            lasers = temp;
        } else {
            printf("Erreur realloc après suppression (ancienne mémoire conservée)\n");
            // On continue avec l'ancien pointeur
        }
    } else {
        // Plus aucun laser
        free(lasers);
        lasers = NULL;
    }
}

void updateEnemies(Ship **ships, int *shipCount) {
    if (SDL_GetTicks() - lastEnemyGenerationTime < ENEMY_GENERATION_PERIOD) return;

    lastEnemyGenerationTime = SDL_GetTicks();

    // Sauvegarde la taille actuelle pour ne pas boucler sur les ennemis qu'on ajoute pendant la boucle
    int initialShipCount = *shipCount;
    for (int i = 0; i < initialShipCount; i++) {
        if ((*ships)[i].shiptype != ENEMY) {
            generateEnemy((*ships)[i], ships, shipCount);
        }
    }
}


void updateLasers(Ship **ships, int *shipCount, Mix_Chunk **sounds) {
    float dx, dy, distance;

    // Met à jour la position des lasers existants
    for (int i = lasersCount - 1; i >= 0; i--) {
        dx = lasers[i].target_coords.x - lasers[i].rect.x;
        dy = lasers[i].target_coords.y - lasers[i].rect.y;
        distance = dist(dx, dy, 0, 0);

        if (distance <= 10) {
            deleteLaser(i);
        } else {
            lasers[i].rect.x += dx * lasers[i].speed / distance;
            lasers[i].rect.y += dy * lasers[i].speed / distance;
            lasers[i].angle = computeAngleDeg(lasers[i].target_coords.x, lasers[i].target_coords.y, lasers[i].rect.x, lasers[i].rect.y);
        }
    }

    // Génère de nouveaux lasers à intervalle régulier
    if (SDL_GetTicks() - lastLaserGenerationTime  + rand()%LASER_GENERATION_PERIOD/2 >= LASER_GENERATION_PERIOD) {
        lastLaserGenerationTime = SDL_GetTicks();

        for (int i = 0; i < *shipCount; i++) {
            int target_id = (*ships)[i].target.id_ship;

            // Vérifie que la cible est valide
            if (target_id >= 0 && target_id < *shipCount) {
                SDL_Rect rect = {
                    (*ships)[i].x,
                    (*ships)[i].y,
                    200, 200
                };

                SDL_Point target_coords = {
                    (*ships)[target_id].x,
                    (*ships)[target_id].y
                };

                addLaser(rect, target_coords, 0, 20);
                Mix_PlayChannel(1, sounds[5], 0);
            } else {
                fprintf(stderr, "ID cible invalide : %d (shipCount = %d)\n", target_id, *shipCount);
            }
        }
    }
}


void renderLasers(SDL_Texture ***imageTextures) {
    SDL_Rect dstRect;
    for (int i = 0; i<lasersCount; i++) {
        dstRect.x = (lasers[i].rect.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
        dstRect.y = (lasers[i].rect.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
        dstRect.w = lasers[i].rect.w * getCameraScale();
        dstRect.h = lasers[i].rect.h * getCameraScale();
        SDL_RenderCopyEx(renderer, imageTextures[5][10], NULL, &dstRect, lasers[i].angle, NULL, SDL_FLIP_NONE);
    }
}

void generateEnemy(Ship targetShip, Ship **ships, int *shipCount) {
    Ship enemyShip;
    enemyShip.shiptype = ENEMY;
    enemyShip.id = *shipCount;
    enemyShip.idModel = 6;
    
    enemyShip.target.type = SPOT_SHIP;
    enemyShip.target.id_ship = targetShip.id;
    enemyShip.state = ATTACKING_SHIP;

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
    enemyShip.noise = 50;

    enemyShip.waitStartTime = 0;
    enemyShip.frameIndex = rand() % 4;  // Desynchronisation des fusees
    enemyShip.lastFrameTime = 0;
    enemyShip.lastRefreshFilling = SDL_GetTicks();

    enemyShip.destRect.x = 0;
    enemyShip.destRect.y = 0;
    enemyShip.destRect.w = 0;
    enemyShip.destRect.h = 0;
    enemyShip.angleWithPlanet = 0;

    addShip(enemyShip, ships, shipCount);
}
