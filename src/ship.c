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
        (*ships)[i].base = &planets[0];  // La première planète est la base de chaque vaisseau
        (*ships)[i].target = &planets[rand() % planetCount];
        (*ships)[i].x = (*ships)[i].base->x;
        (*ships)[i].y = (*ships)[i].base->y;
        (*ships)[i].speed = (rand() / (float)RAND_MAX + 0.01) * SHIP_SPEED;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].maxLife = 100;
        (*ships)[i].currentLife = rand() % (int)(*ships)[i].maxLife;

        (*ships)[i].waitStartTime = 0;
        (*ships)[i].frameIndex = rand() % 4;  // Desynchronisation des fusees
        (*ships)[i].lastFrameTime = 0;
        (*ships)[i].fuelConsumption = 5;  // Consommation d'essence par intervalle de temps FUEL_UPDATE_INTERVAL
        (*ships)[i].lastRefreshFuel = SDL_GetTicks();

        // Allocation des compartiments
        (*ships)[i].cargo.compartmentsNumber = 3;
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
        }
    }
}

void updateShips(Ship *ships, int shipCount, Planet *planets, int planetCount) { 
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        updateShipAnimation(&ships[i], currentTime);  // Permet de changer de frame du sprite sheet de la fusee i
        updateShipMove(&ships[i], planets, planetCount, currentTime);  // Actualise le mouvement de la fusee i
        updateShipFuel(&ships[i], currentTime);  // Gère la consommation d'essence de la fusee i
    }
}

void updateShipAnimation(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    if (currentTime > ship->lastFrameTime + SHIP_FRAME_DELAY) {
        ship->frameIndex = (ship->frameIndex + 1) % 4;  // 4 images dans le sprite sheet
        ship->lastFrameTime = currentTime;
    }
}

void updateShipMove(Ship *ship, Planet *planets, int planetCount, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    // Pour le deplacement des fusees dans l'espace
    if (ship->state == MOVING_TO_TARGET || ship->state == RETURNING) {
        Planet *dest = (ship->state == MOVING_TO_TARGET) ? ship->target : ship->base;
        float dx = dest->x - (ship->x + 64 / 2.f);
        float dy = dest->y - (ship->y + 64 / 2.f);
        float distance = sqrt(dx * dx + dy * dy);

        if (distance - ship->speed >= dest->radius) {
            ship->x += dx * ship->speed / distance;
            ship->y += dy * ship->speed / distance;
        } else {
            if (ship->state == MOVING_TO_TARGET) {
                ship->state = WAITING;
                ship->waitStartTime = currentTime;
            } else if (ship->state == RETURNING) {
                ship->state = WAITING;
                ship->waitStartTime = currentTime;
            } else {
                ship->target = &planets[rand() % planetCount];
                ship->state = MOVING_TO_TARGET;
            }
        }
    } else if (ship->state == WAITING) {
        if (currentTime - ship->waitStartTime > WAIT_TIME_SHIP) {
            ship->lastRefreshFuel = currentTime;  // La fusee decolle, actualisation de lastRefreshFuel
            if (carre(ship->target->x - ship->x) + carre(ship->target->x - ship->x) >
                carre(ship->base->x - ship->x) + carre(ship->base->x - ship->x))  {
                ship->state = MOVING_TO_TARGET;
            } else {
                ship->state = RETURNING;
            }
        }
    }
}

void updateShipFuel(Ship *ship, Uint32 currentTime) {  // Gere la consommation d'essence
    if (currentTime - ship->lastRefreshFuel >= FUEL_UPDATE_INTERVAL) {  // Actualisation chaque seconde
        ship->lastRefreshFuel += FUEL_UPDATE_INTERVAL;
        if ((ship->state == MOVING_TO_TARGET || ship->state == RETURNING)) {  // Cas ou la fusee est en mouvement
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
        }
    }
}

void renderShips(SDL_Renderer *renderer, SDL_Texture *spriteSheet, Ship *ships, int count) {
    for (int i = 0; i < count; i++) {
        // Calcul des coordonnees a l'ecran, du point en haut a gauche de la fusee
        SDL_Point ShipOnScreen = {(ships[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f,
                                  (ships[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f};

        if (ShipOnScreen.x >= -64 * camera.scale && ShipOnScreen.x <= SCREEN_WIDTH && 
            ShipOnScreen.y >= -64 * camera.scale && ShipOnScreen.y <= SCREEN_HEIGHT + 64 * camera.scale) {  // Si la fusee est dans l'ecran 
            renderShipImage(renderer, spriteSheet, ships[i], ShipOnScreen);
            renderShipBars(renderer, ships[i], ShipOnScreen);
        }
    }
}

void renderShipImage(SDL_Renderer *renderer, SDL_Texture *spriteSheet, Ship ship, SDL_Point ShipOnScreen) {
    // Calcul de l'angle en degres de l'image
    float angle = atan2(ship.target->y - (ship.y + 64 / 2.f), ship.target->x - (ship.x + 64 / 2.f)) * 180.0f / M_PI;
    angle += (ship.state == MOVING_TO_TARGET) ? 90.0f : -90.0f;  // Si la fusée ne va pas vers la cible, on l'inverse

    SDL_Rect srcRect = {ship.frameIndex * 64, 0, 64, 64};  // Frame actuelle sur le sprite sheet
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y, 64 * camera.scale, 64 * camera.scale};  // Position et taille affichee
    SDL_Point center = {destRect.w / 2, destRect.h / 2};  // Définition du point de rotation (au centre du sprite)

    // Dessin des fusees avec rotation
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
}


void renderShipBars(SDL_Renderer *renderer, Ship ship, SDL_Point ShipOnScreen) {
    // Dessin de la barre d'essence (1)
    float heightBar = 1 * camera.scale;
    float gapBar = 2 * camera.scale;
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y - heightBar - gapBar, 64 * camera.scale, heightBar};  // Rect de la barre

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &destRect);

    // Dessin de la barre d'essence (2)
    int nbFuelCompartment = 0;
    int firstFuelCompartment;
    for (int i = ship.cargo.compartmentsNumber - 1; i >= 0; i--) {  // firstFuelCompartment permet d'afficher la bonne barre
        if (ship.cargo.compartmentsList[i].ore == FUEL &&           // d'essence, celle qui est modifiee lors du deplacement.
            ship.cargo.compartmentsList[i].currentCapacity > 0) {           
            nbFuelCompartment ++;
            firstFuelCompartment = i;
        }
    }

    if (nbFuelCompartment == 0) {
        destRect.w = 0;
    } else {
        destRect.w *= ship.cargo.compartmentsList[firstFuelCompartment].currentCapacity / (float)ship.cargo.compartmentsList[firstFuelCompartment].maxCapacity;
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &destRect);
}

void destroyShips(Ship *ships, int ship_count) {
    for (int i = 0; i < ship_count; i++) {
        free(ships[i].cargo.compartmentsList);
    }
    free(ships);
}
