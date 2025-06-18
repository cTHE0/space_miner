#include <SDL2/SDL.h>
#include "enemy.h"
#include "ship.h"
#include "config.h"

static Uint32 lastEnemyGenerationTime = 0;

void generateEnemies(Ship **ships, int *shipCount) {
    Uint32 now = SDL_GetTicks();
    if (now - lastEnemyGenerationTime > ENEMY_GENERATION_PERIOD){
        lastEnemyGenerationTime = now;
        int nb_ships = *shipCount;
        for (int i=0; i< nb_ships; i++) {
            if ((*ships)[i].shiptype != ENEMY) {
                generateEnemy((*ships)[i], ships, shipCount);
            }
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
    enemyShip.state = BROKE;
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

    // Allocation des compartiments
    Cargo *cargo = &enemyShip.cargo;

    cargo->compartmentsNumber = 3;
    cargo->compartmentsList = malloc(cargo->compartmentsNumber * sizeof(Compartment));
    if (cargo->compartmentsList == NULL) {
        printf("Erreur d'allocation memoire pour les compartiments du vaisseau %d!\n", *shipCount);
        
        // Liberer la memoire des vaisseaux deja crees
        for (int j = 0; j < *shipCount; j++) {
            free(cargo->compartmentsList);
        }
        free(*ships);
        return;
    }
    for (int j = 0; j < enemyShip.cargo.compartmentsNumber; j++) {  // Ici, chaque compartiment contient de l'essence
        cargo->compartmentsList[j].ore = rand() % 4;
        cargo->compartmentsList[j].maxCapacity = 1000;
        cargo->compartmentsList[j].currentCapacity = 700;
        cargo->compartmentsList[j].flowSpeed = 20;
        cargo->compartmentsList[j].level = 1;

        // Pour tester le systeme de ressource
        // Faire une interface graphique pour gerer ca proprement
        if (cargo->compartmentsList[j].ore == FUEL) {
            cargo->compartmentsList[j].flowBase_in = FUEL;
            cargo->compartmentsList[j].flowBase_out = EMPTY;
            cargo->compartmentsList[j].flowTarget_in = FUEL;
            cargo->compartmentsList[j].flowTarget_out = EMPTY;
        } else {
            cargo->compartmentsList[j].flowBase_in = EMPTY;
            cargo->compartmentsList[j].flowBase_out = cargo->compartmentsList[j].ore;
            cargo->compartmentsList[j].flowTarget_in = cargo->compartmentsList[j].ore;
            cargo->compartmentsList[j].flowTarget_out = EMPTY;
        }
    }

    addShip(&enemyShip, ships, shipCount);
}