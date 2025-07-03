#include <SDL2/SDL.h>
#include "enemy.h"
#include "ship.h"
#include "config.h"

static Uint32 lastEnemyGenerationTime = 0;

void updateEnemies(Ship **ships, int *shipCount) {
    if (SDL_GetTicks() - lastEnemyGenerationTime < ENEMY_GENERATION_PERIOD) {
        return;
    }

    lastEnemyGenerationTime = SDL_GetTicks();
    int nb_ships = *shipCount;
    for (int i = 0; i < nb_ships; i++) {
        if ((*ships)[i].shiptype != ENEMY) {
            generateEnemy((*ships)[i], ships, shipCount);
        }
    }
}

void generateEnemy(Ship targetShip, Ship **ships, int *shipCount) {
    Ship enemyShip;
    enemyShip.shiptype = ENEMY;
    enemyShip.id = *shipCount;
    enemyShip.idModel = 6;
    enemyShip.base.type = SPOT_NONE;
    enemyShip.target.type = SPOT_NONE;
    enemyShip.x = targetShip.x + rand() % 5000 - 2500;
    enemyShip.y = targetShip.y + rand() % 5000 - 2500;
    enemyShip.w = 200;
    enemyShip.h = 200;
    enemyShip.speed = (rand() / (float)RAND_MAX * 0.6 + 0.4) * SHIP_SPEED;
    enemyShip.level = 1;
    enemyShip.state = BROKEN;
    enemyShip.maxLife = 100;
    enemyShip.currentLife = 100;
    enemyShip.fuelConsumption = 1;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
    enemyShip.range = 300;
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
