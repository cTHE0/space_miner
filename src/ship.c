#include "ship.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "planet.h"
#include "camera.h"
#include "config.h"
#include "renderer.h"


void initShips(Ship **ships, int shipCount, Planet *planets, int planetCount) {
    *ships = malloc(shipCount * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation memoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour eviter d'utiliser ships apres un echec
    }

    for (int i = 0; i < shipCount; i++) {
        (*ships)[i].shiptype = TRANSPORTER;
        (*ships)[i].id = i;
        (*ships)[i].idModel = rand() % 12;
        (*ships)[i].base.type = SPOT_PLANET;
        (*ships)[i].base.planet = &planets[1];  // La premiere planete est la base de chaque vaisseau
        (*ships)[i].base.id_spot = 1;
        (*ships)[i].target.type = SPOT_PLANET;
        (*ships)[i].target.id_spot = rand() % (planetCount - 2) + 2;
        (*ships)[i].target.planet = &planets[(*ships)[i].target.id_spot];
        (*ships)[i].x = (*ships)[i].base.planet->x;
        (*ships)[i].y = (*ships)[i].base.planet->y;
        (*ships)[i].w = 62;
        (*ships)[i].h = 62;
        (*ships)[i].speed = (rand() / (float)RAND_MAX * 0.6 + 0.4) * SHIP_SPEED;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].maxLife = 100;
        (*ships)[i].currentLife = rand() % (int)(*ships)[i].maxLife;
        (*ships)[i].fuelConsumption = 1;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
        (*ships)[i].range = 300;

        (*ships)[i].waitStartTime = 0;
        (*ships)[i].frameIndex = rand() % 4;  // Desynchronisation des fusees
        (*ships)[i].lastFrameTime = 0;
        (*ships)[i].lastRefreshFilling = SDL_GetTicks();

        (*ships)[i].destRect.x = 0;
        (*ships)[i].destRect.y = 0;
        (*ships)[i].destRect.w = 0;
        (*ships)[i].destRect.h = 0;

        // Allocation des compartiments
        Cargo *cargo = &(*ships)[i].cargo;

        cargo->compartmentsNumber = 3;
        cargo->compartmentsList = malloc(cargo->compartmentsNumber * sizeof(Compartment));
        if (cargo->compartmentsList == NULL) {
            printf("Erreur d'allocation memoire pour les compartiments du vaisseau %d!\n", i);
            
            // Liberer la memoire des vaisseaux deja crees
            for (int j = 0; j < i; j++) {
                free(cargo->compartmentsList);
            }
            free(*ships);
            return;
        }
        for (int j = 0; j < (*ships)[i].cargo.compartmentsNumber; j++) {  // Ici, chaque compartiment contient de l'essence
            cargo->compartmentsList[j].ore = rand() % 4;
            cargo->compartmentsList[j].maxCapacity = 1000;
            cargo->compartmentsList[j].currentCapacity = 1000;
            cargo->compartmentsList[j].flowSpeed = 20;

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
    }
}

void updateShips(Ship *ships, int shipCount) { 
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        updateShipAnimation(&ships[i], currentTime);  // Permet de changer de frame du sprite sheet de la fusee i
        updateShipMove(&ships[i], currentTime);
        updateShipTanks(&ships[i], currentTime);
    }
}

void updateShipAnimation(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    if (currentTime > ship->lastFrameTime + SPRITE_SHEETS_DELAY) {
        ship->frameIndex = (ship->frameIndex + 1) % 4;  // 4 images dans le sprite sheet
        ship->lastFrameTime = currentTime;
    }
}

void updateShipMove(Ship *ship, Uint32 currentTime) {
    if (ship->state != MOVING_TO_BASE && ship->state != MOVING_TO_TARGET) { // La fusee bouge-t-elle ?
        return;
    }

    float dx;
    float dy;
    float distance;
    Spot spotDest = (ship->state == MOVING_TO_BASE) ? ship->base : ship->target;
    switch (spotDest.type) {
        case SPOT_PLANET:
            dx = spotDest.planet->x - (ship->x + ship->w / 2.);
            dy = spotDest.planet->y - (ship->y + ship->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - ship->speed >= spotDest.planet->radius) {
                ship->x += dx * ship->speed / distance;
                ship->y += dy * ship->speed / distance;
            } else {
                ship->waitStartTime = currentTime;
                ship->state = (ship->state == MOVING_TO_BASE) ? WAITING_ON_BASE : WAITING_ON_TARGET;
            }
            break;
        case SPOT_SHIP:
            dx = spotDest.ship->x + spotDest.ship->w / 2. - (ship->x + ship->w / 2.);
            dy = spotDest.ship->y + spotDest.ship->h / 2. - (ship->y + ship->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - ship->speed >= (spotDest.ship->h + ship->h) / 2) {
                ship->x += dx * ship->speed / distance;
                ship->y += dy * ship->speed / distance;
            } else {
                ship->waitStartTime = currentTime;
                ship->state = (ship->state == MOVING_TO_BASE) ? WAITING_ON_BASE : WAITING_ON_TARGET;
            }
            break;
        case SPOT_POINT:
            dx = spotDest.point.x - (ship->x + ship->w / 2.);
            dy = spotDest.point.y - (ship->y + ship->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - ship->speed > 0) {
                ship->x += dx * ship->speed / distance;
                ship->y += dy * ship->speed / distance;
            } else {
                ship->waitStartTime = currentTime;
                ship->state = (ship->state == MOVING_TO_BASE) ? MOVING_TO_TARGET : MOVING_TO_BASE;
            }
            break;
        default:
            break;
    }
}

void updateShipTanks(Ship *ship, Uint32 currentTime) {  // Gere depot/recuperation des minerais/essence, et consommation essence
    if (currentTime - ship->lastRefreshFilling < TANKS_UPDATE_INTERVAL) {  // Actualisation chaque seconde
        return;
    }

    ship->lastRefreshFilling += TANKS_UPDATE_INTERVAL;
    if ((ship->state == MOVING_TO_TARGET || ship->state == MOVING_TO_BASE)) {  // Cas ou la fusee est en mouvement
        fuelConsumption(ship);
    } else if (ship->state == WAITING_ON_BASE || ship->state == WAITING_ON_TARGET) {
        if ((ship->state == WAITING_ON_BASE && ship->base.type != SPOT_PLANET) || 
            (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET)) {  // Pas de transfert de ressource ailleurs que sur une planet (actuellement !)
            return;
        }
        OreFillingOrEmptying(ship);
    }
}

void fuelConsumption(Ship *ship) {
    int HaveFuel = 0;  // 1: La fusee a de l'essence, 0: la fusee n'en a plus

    for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
        if (ship->cargo.compartmentsList[i].ore == FUEL &&
            ship->cargo.compartmentsList[i].currentCapacity > 0) {
            HaveFuel = 1;
            ship->cargo.compartmentsList[i].currentCapacity -= ship->fuelConsumption;
            if (ship->cargo.compartmentsList[i].currentCapacity < 0) {
                ship->cargo.compartmentsList[i].currentCapacity = 0;
            }
            break;
        }
    }
    
    if (HaveFuel == 0) {
        ship->state = OUT_OF_FUEL;
    }
}

void OreFillingOrEmptying(Ship *ship) {
    if ((ship->state == WAITING_ON_BASE && ship->base.type != SPOT_PLANET) ||
        (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET)) {  // La fusee est-elle sur une planete ?
        return;
    }

    int modified = 0;  // Verifie s'il s'est passe qch (1:oui, 0:non)
    Cargo *cargo = &ship->cargo;
    Planet *landingPlanet = (ship->state == WAITING_ON_BASE) ? ship->base.planet : ship->target.planet;

    for (int i = 0; i < cargo->compartmentsNumber; i++) {
        if (ship->state == WAITING_ON_BASE) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowBase_in != EMPTY &&  // Eviter ce cas illogique
                landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_in] > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_in)) {  // La fusee peut recuperer les ressources

                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_in] -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_in] < 0.f) {
                    landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_in] = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowBase_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_out] < landingPlanet->maxOre[cargo->compartmentsList[i].flowBase_out] &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_out] += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_out] < landingPlanet->maxOre[cargo->compartmentsList[i].flowBase_out]) {
                    landingPlanet->currentOre[cargo->compartmentsList[i].flowBase_in] = landingPlanet->maxOre[cargo->compartmentsList[i].flowBase_out];
                }
                modified = 1;
                break;
            }

        } else if (ship->state == WAITING_ON_TARGET) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowTarget_in != EMPTY &&  // Eviter ce cas illogique
                cargo->compartmentsList[i].currentCapacity < cargo->compartmentsList[i].maxCapacity &&  // Les reservoirs d'essence de la fusee sont remplis avant
                landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_in] > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_in)) {  // La fusee peut recuperer les ressources
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_in] -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_in] < 0.f) {
                    landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_in] = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowTarget_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_out] < landingPlanet->maxOre[cargo->compartmentsList[i].flowTarget_out] &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_out] += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_out] < landingPlanet->maxOre[cargo->compartmentsList[i].flowTarget_out]) {
                    landingPlanet->currentOre[cargo->compartmentsList[i].flowTarget_in] = landingPlanet->maxOre[cargo->compartmentsList[i].flowTarget_out];
                }
                modified = 1;
                break;
            }
        }
    }

    // Si les transferts de matieres sont finis, changer l'etat de la fusee
    if (!modified) {
        ship->state = (ship->state == WAITING_ON_BASE) ? MOVING_TO_TARGET : MOVING_TO_BASE;
    }       
}

void renderShips(SDL_Texture ***imageTextures, Ship *ships, int shipCount) {
    for (int i = 0; i < shipCount; i++) {
        // Calcul des coordonnees a l'ecran, du point en haut a gauche de la fusee
        SDL_Point ShipOnScreen = (SDL_Point){(ships[i].x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                             (ships[i].y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};

        if (ShipOnScreen.x >= -ships[i].w * getCameraScale() && ShipOnScreen.x <= SCREEN_WIDTH && 
            ShipOnScreen.y >= -ships[i].h * getCameraScale() && ShipOnScreen.y <= SCREEN_HEIGHT + ships[i].h * getCameraScale()) {  // Si la fusee est dans l'ecran 
            renderShipImage(imageTextures[7][ships[i].idModel], ships[i], ShipOnScreen);
            renderShipBars(ships[i], ShipOnScreen);
            ships[i].destRect.x = ShipOnScreen.x;
            ships[i].destRect.y = ShipOnScreen.y;
            ships[i].destRect.w = ships[i].w * getCameraScale();
            ships[i].destRect.h = ships[i].h * getCameraScale();
        }
    }
}

void renderShipImage(SDL_Texture *textureShip, Ship ship, SDL_Point ShipOnScreen) {
    // Calcul de l'angle en degres de l'image de la fusee
    float angle;
    Spot spotDest = (ship.state == WAITING_ON_BASE || ship.state == MOVING_TO_BASE) ? ship.base : ship.target;

    switch (spotDest.type) {
        case SPOT_PLANET:
            angle = atan2(spotDest.planet->y - (ship.y + ship.h / 2.f), spotDest.planet->x - (ship.x + ship.w / 2.f)) * 180.0f / M_PI;
            break;
        case SPOT_SHIP:
            angle = atan2((spotDest.ship->y + spotDest.ship->h / 2.f) - (ship.y + ship.h / 2.f), (spotDest.ship->x + spotDest.ship->w) - (ship.x + ship.w / 2.f)) * 180.0f / M_PI;
            break;
        case SPOT_POINT:
            angle = atan2(spotDest.point.y - (ship.y + ship.h / 2.f), spotDest.point.x - (ship.x + ship.w / 2.f)) * 180.0f / M_PI;
            break;
        default:
            angle = 0;
            break;
    }

    // Ajuste l'angle en fonction du sens de deplacement
    angle +=(ship.state == MOVING_TO_BASE || ship.state == MOVING_TO_TARGET) ? 90 : -90;

    // Creation des variables necessaires a l'affichage
    SDL_Rect srcRect = {ship.frameIndex * 64, 0, 64, 64};  // Frame actuelle sur le sprite sheet
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y, ship.w * getCameraScale(), ship.h * getCameraScale()};  // Position et taille affichee
    SDL_Point center = {destRect.w / 2, destRect.h / 2};  // Definition du point de rotation (au centre du sprite)

    // Dessin des fusees avec rotation
    SDL_RenderCopyEx(renderer, textureShip, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
}


void renderShipBars(Ship ship, SDL_Point ShipOnScreen) {
    // Dessin de la barre d'essence (1)
    float heightBar = 1 * getCameraScale();
    float gapBar = 2 * getCameraScale();
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y - heightBar - gapBar, 64 * getCameraScale(), heightBar};  // Rect de la barre

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &destRect);

    // Dessin de la barre d'essence (2)
    int fuelRemaining = 0;  // O: Il n'y a plus d'essence dans les reservoirs, 1 sinon
    int CompartmentToDisplay = -1;  // Permet d'afficher celui qui varie
    Cargo cargo = ship.cargo;
    for (int i = 0; i < cargo.compartmentsNumber; i++) {
        if (cargo.compartmentsList[i].ore == FUEL &&
            cargo.compartmentsList[i].currentCapacity > 0) {   
            fuelRemaining = 1;                 
            CompartmentToDisplay = i;
            if (cargo.compartmentsList[i].currentCapacity < cargo.compartmentsList[i].maxCapacity) {
                break;
            }
        }
    }

    if (fuelRemaining == 1) {
        destRect.w *= cargo.compartmentsList[CompartmentToDisplay].currentCapacity / (float)cargo.compartmentsList[CompartmentToDisplay].maxCapacity;
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
