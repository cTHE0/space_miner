#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "ship.h"
#include "camera.h"
#include "config.h"


void initShips(Ship **ships, int shipCount, Planet *planets, int planetCount) {
    *ships = malloc(shipCount * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation mémoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *ships après un échec
    }

    for (int i = 0; i < shipCount; i++) {
        (*ships)[i].shiptype = TRANSPORTER;
        (*ships)[i].idPicture = rand() % 10;
        (*ships)[i].base = &planets[0];  // La première planète est la base de chaque vaisseau
        (*ships)[i].target = &planets[rand() % (planetCount - 1)] + 1;
        (*ships)[i].x = (*ships)[i].base->x;
        (*ships)[i].y = (*ships)[i].base->y;
        (*ships)[i].speed = (rand() / (float)RAND_MAX * 0.7 + 0.3) * SHIP_SPEED;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].maxLife = 100;
        (*ships)[i].currentLife = rand() % (int)(*ships)[i].maxLife;
        (*ships)[i].fuelConsumption = 1;  // Consommation d'essence par intervalle de temps FUEL_UPDATE_INTERVAL

        (*ships)[i].waitStartTime = 0;
        (*ships)[i].frameIndex = rand() % 4;  // Desynchronisation des fusees
        (*ships)[i].lastFrameTime = 0;
        (*ships)[i].lastRefreshFilling = SDL_GetTicks();

        (*ships)[i].destRect.x = 0;
        (*ships)[i].destRect.y = 0;
        (*ships)[i].destRect.w = 0;
        (*ships)[i].destRect.h = 0;

        // Allocation des compartiments
        (*ships)[i].cargo.compartmentsNumber = 2;
        (*ships)[i].cargo.compartmentsList = malloc((*ships)[i].cargo.compartmentsNumber * sizeof(Compartment));
        if ((*ships)[i].cargo.compartmentsList == NULL) {
            printf("Erreur d'allocation mémoire pour les compartiments du vaisseau %d!\n", i);
            
            // Libérer la mémoire des vaisseaux déjà créés
            for (int j = 0; j < i; j++) {
                free((*ships)[j].cargo.compartmentsList);
            }
            free(*ships);
            *ships = NULL;
            return;
        }
        for (int j = 0; j < (*ships)[i].cargo.compartmentsNumber; j++) {  // Ici, chaque compartiment contient de l'essence
            (*ships)[i].cargo.compartmentsList[j].ore = FUEL;
            (*ships)[i].cargo.compartmentsList[j].maxCapacity = 100;
            (*ships)[i].cargo.compartmentsList[j].currentCapacity = 100;
            (*ships)[i].cargo.compartmentsList[j].flowSpeed = 5;
        }
    }
}

void updateShips(Ship *ships, int shipCount) { 
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        updateShipAnimation(&ships[i], currentTime);  // Permet de changer de frame du sprite sheet de la fusee i
        updateShipMove(&ships[i], currentTime);  // Actualise le mouvement de la fusee i
        updateShipFuel(&ships[i], currentTime);  // Gère la consommation d'essence de la fusee i
    }
}

void updateShipAnimation(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    if (currentTime > ship->lastFrameTime + SHIP_FRAME_DELAY) {
        ship->frameIndex = (ship->frameIndex + 1) % 4;  // 4 images dans le sprite sheet
        ship->lastFrameTime = currentTime;
    }
}

void updateShipMove(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    // Pour le deplacement des fusees dans l'espace
    if (ship->state == MOVING_TO_TARGET || ship->state == MOVING_TO_BASE) {
        Planet *dest = (ship->state == MOVING_TO_TARGET) ? ship->target : ship->base;
        float dx = dest->x - (ship->x + 64 / 2.f);
        float dy = dest->y - (ship->y + 64 / 2.f);
        float distance = sqrt(dx * dx + dy * dy);

        if (distance - ship->speed >= dest->radius) {
            ship->x += dx * ship->speed / distance;
            ship->y += dy * ship->speed / distance;
        } else {
            ship->waitStartTime = currentTime;
            ship->state = (ship->state == MOVING_TO_BASE) ? WAITING_ON_BASE : WAITING_ON_TARGET;
        }
    } else if (ship->state == WAITING_ON_TARGET) {  // Partie a supprimer qd les fusees pourront miner
        if (currentTime - ship->waitStartTime > WAIT_TIME_SHIP) {
            ship->state = MOVING_TO_BASE; 
            ship->lastRefreshFilling = currentTime;
        }
    }
}

void updateShipFuel(Ship *ship, Uint32 currentTime) {  // Gere la consommation d'essence
    if (currentTime - ship->lastRefreshFilling >= FUEL_UPDATE_INTERVAL) {  // Actualisation chaque seconde
        ship->lastRefreshFilling += FUEL_UPDATE_INTERVAL;
        if ((ship->state == MOVING_TO_TARGET || ship->state == MOVING_TO_BASE)) {  // Cas ou la fusee est en mouvement
            int i = 0;
            int HaveFuel = 0;  // 1: La fusee a de l'essence, 0: la fusee n'en a plus
            while (i < ship->cargo.compartmentsNumber) {
                if (ship->cargo.compartmentsList[i].ore == FUEL &&
                    ship->cargo.compartmentsList[i].currentCapacity > 0) {
                    HaveFuel = 1;
                    ship->cargo.compartmentsList[i].currentCapacity -= ship->fuelConsumption;
                    if (ship->cargo.compartmentsList[i].currentCapacity < 0) {
                        ship->cargo.compartmentsList[i].currentCapacity = 0;
                    }
                    break;
                }
                i ++;
            }
            if (HaveFuel == 0) {
                ship->state = OUT_OF_FUEL;
            }
        } else if (ship->state == WAITING_ON_BASE) {
            int fullyFuelFilled = 1;  // 1: La fusée a fait le plein, 0: plein en cours
            int lastEmptyFuelCompartment = -1;  // Permet de ne remplir que le dernier réservoir vide
            int fuelGiven = 0;  // Vérifie si de l'essence a été donnée (1:oui, 0:non)
            for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
                if (ship->cargo.compartmentsList[i].ore == FUEL &&
                    ship->cargo.compartmentsList[i].currentCapacity < ship->cargo.compartmentsList[i].maxCapacity) {
                    lastEmptyFuelCompartment = i;
                    fullyFuelFilled = 0;
                    if (ship->cargo.compartmentsList[i].currentCapacity > 0) {
                        ship->cargo.compartmentsList[i].currentCapacity += ship->cargo.compartmentsList[i].flowSpeed;
                        fuelGiven = 1;
                        if (ship->cargo.compartmentsList[i].currentCapacity >= ship->cargo.compartmentsList[i].maxCapacity) {
                            ship->cargo.compartmentsList[i].currentCapacity = ship->cargo.compartmentsList[i].maxCapacity;
                        }
                        break;
                    }
                }
            }

            // Si aucune essence n'a été donnée, remplir le dernier compartiment vide
            if (!fuelGiven && lastEmptyFuelCompartment != -1) {
                ship->cargo.compartmentsList[lastEmptyFuelCompartment].currentCapacity += ship->cargo.compartmentsList[lastEmptyFuelCompartment].flowSpeed;
            }

            // Vérifier si tous les compartiments sont pleins
            for (int i = 0; i < ship->cargo.compartmentsNumber && fullyFuelFilled; i++) {
                if (ship->cargo.compartmentsList[i].ore == FUEL &&
                    ship->cargo.compartmentsList[i].currentCapacity < ship->cargo.compartmentsList[i].maxCapacity) {
                    fullyFuelFilled = 0;
                }
            }

            // Si tous les réservoirs sont pleins, changer l'état du vaisseau
            if (fullyFuelFilled) {
                ship->state = MOVING_TO_TARGET;
            }
        }
    }
}

void renderShips(SDL_Renderer *renderer, SDL_Texture ***textureShip, Ship *ships, int shipCount) {
    for (int i = 0; i < shipCount; i++) {
        // Calcul des coordonnees a l'ecran, du point en haut a gauche de la fusee
        SDL_Point ShipOnScreen = {(ships[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f,
                                  (ships[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f};

        if (ShipOnScreen.x >= -64 * camera.scale && ShipOnScreen.x <= SCREEN_WIDTH && 
            ShipOnScreen.y >= -64 * camera.scale && ShipOnScreen.y <= SCREEN_HEIGHT + 64 * camera.scale) {  // Si la fusee est dans l'ecran 
            renderShipImage(renderer, textureShip[0][ships[i].idPicture], ships[i], ShipOnScreen);
            renderShipBars(renderer, ships[i], ShipOnScreen);
            ships[i].destRect.x = ShipOnScreen.x;
            ships[i].destRect.y = ShipOnScreen.y;
            ships[i].destRect.w = 64 * camera.scale;
            ships[i].destRect.h = 64 * camera.scale;
        }
    }
}

void renderShipImage(SDL_Renderer *renderer, SDL_Texture *textureShip, Ship ship, SDL_Point ShipOnScreen) {
    // Calcul de l'angle en degres de l'image
    float angle = atan2(ship.target->y - (ship.y + 64 / 2.f), ship.target->x - (ship.x + 64 / 2.f)) * 180.0f / M_PI;

    if (ship.state == MOVING_TO_TARGET || ship.state == WAITING_ON_BASE) {  // Pour que les fusees atterissent dans le bon sens
        angle += 90;
    } else if (ship.state == MOVING_TO_BASE || ship.state == WAITING_ON_TARGET) {
        angle -= 90;
    }

    SDL_Rect srcRect = {ship.frameIndex * 64, 0, 64, 64};  // Frame actuelle sur le sprite sheet
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y, 64 * camera.scale, 64 * camera.scale};  // Position et taille affichee
    SDL_Point center = {destRect.w / 2, destRect.h / 2};  // Définition du point de rotation (au centre du sprite)

    // Dessin des fusees avec rotation
    SDL_RenderCopyEx(renderer, textureShip, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
}


void renderShipBars(SDL_Renderer *renderer, Ship ship, SDL_Point ShipOnScreen) {
    // Dessin de la barre d'essence (1)
    float heightBar = 1 * camera.scale;
    float gapBar = 2 * camera.scale;
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y - heightBar - gapBar, 64 * camera.scale, heightBar};  // Rect de la barre

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &destRect);

    // Dessin de la barre d'essence (2)
    int fuelRemaining = 0;  // O: Il n'y a plus d'essence dans les reservoirs, 1 sinon
    int CompartmentToDisplay = -1;  // Permet d'afficher celui qui varie
    int i;
    for (i = 0; i < ship.cargo.compartmentsNumber; i++) {
        if (ship.cargo.compartmentsList[i].ore == FUEL &&
            ship.cargo.compartmentsList[i].currentCapacity > 0) {   
            fuelRemaining = 1;                 
            CompartmentToDisplay = i;
            if (ship.cargo.compartmentsList[i].currentCapacity < ship.cargo.compartmentsList[i].maxCapacity) {
                break;
            }
        }
    }

    if (fuelRemaining == 1) {
        destRect.w *= ship.cargo.compartmentsList[CompartmentToDisplay].currentCapacity / (float)ship.cargo.compartmentsList[CompartmentToDisplay].maxCapacity;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }

}

void destroyShips(Ship *ships, int shipCount) {
    for (int i = 0; i < shipCount; i++) {
        free(ships[i].cargo.compartmentsList);
    }
    free(ships);
}
