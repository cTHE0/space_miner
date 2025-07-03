#include "ship.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "planet.h"
#include "camera.h"
#include "config.h"
#include "renderer.h"
#include "tools.h"
#include "enemy.h"

void addShip(Ship newShip, Ship **ships, int *shipCount) {
    *ships = realloc(*ships, (*shipCount + 1) * sizeof(Ship));
    if (*ships == NULL) {
        exit(1);
    }

    (*ships)[*shipCount] = newShip;
    (*shipCount) ++;
}

void initShips(Ship **ships, int shipCount, Planet *planets) {
    *ships = malloc(shipCount * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation memoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour eviter d'utiliser ships apres un echec
    }

    for (int i = 0; i < shipCount; i++) {
        (*ships)[i].shiptype = TRANSPORTER;
        (*ships)[i].id = i;
        (*ships)[i].idModel = rand() % 7;
        (*ships)[i].base.type = SPOT_PLANET;
        (*ships)[i].base.id_planet = 1;  // La premiere planete est la base de chaque vaisseau
        (*ships)[i].target.type = SPOT_PLANET;
        (*ships)[i].target.id_planet = 2;
        (*ships)[i].x = planets[(*ships)[i].base.id_planet].x;
        (*ships)[i].y = planets[(*ships)[i].base.id_planet].y;
        (*ships)[i].w = 200;
        (*ships)[i].h = 200;
        (*ships)[i].speed = (rand() / (float)RAND_MAX * 0.6 + 0.4) * SHIP_SPEED;
        (*ships)[i].level = 1;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].maxLife = 100;
        (*ships)[i].currentLife = rand() % (int)(*ships)[i].maxLife;
        (*ships)[i].fuelConsumption = 1;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
        (*ships)[i].range = 300;
        (*ships)[i].noise = 50;

        (*ships)[i].waitStartTime = 0;
        (*ships)[i].frameIndex = rand() % 4;  // Desynchronisation des fusees
        (*ships)[i].lastFrameTime = 0;
        (*ships)[i].lastRefreshFilling = SDL_GetTicks();

        (*ships)[i].destRect.x = 0;
        (*ships)[i].destRect.y = 0;
        (*ships)[i].destRect.w = 0;
        (*ships)[i].destRect.h = 0;
        (*ships)[i].angleWithPlanet = 0;

        // Allocation des compartiments
        Cargo *cargo = &(*ships)[i].cargo;

        cargo->compartmentsNumber = 3;
        for (int j = 0; j < (*ships)[i].cargo.compartmentsNumber; j++) {  // Ici, chaque compartiment contient de l'essence
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
    }
}

void updateShips(Ship *ships, Planet *planets, int shipCount) { 
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < shipCount; i++) {
        updateShipAnimation(&ships[i], currentTime);  // Permet de changer de frame du sprite sheet de la fusee i
        updateShipMove(ships, &ships[i], planets, currentTime);
        updateShipTanks(&ships[i], planets, currentTime);
    }
}

void updateShipAnimation(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    if (currentTime > ship->lastFrameTime + SPRITE_SHEETS_DELAY) {
        ship->frameIndex = (ship->frameIndex + 1) % 4;  // 4 images dans le sprite sheet
        ship->lastFrameTime = currentTime;
    }
}

void updateShipMove(Ship *ships, Ship *currentShip, Planet *planets, Uint32 currentTime) {
    if (currentShip->state != MOVING_TO_BASE && currentShip->state != MOVING_TO_TARGET && currentShip->state !=ATTACKING_SHIP) { // La fusee bouge-t-elle ?
        return;
    }

    float dx;
    float dy;
    float distance;
    // Si currentShip->state == MOVING_TO_TARGET OU ATTACKING_SHIP alors la dest est stockée dans currentShip->target
    Spot spotDest = (currentShip->state == MOVING_TO_BASE) ? currentShip->base : currentShip->target;
    switch (spotDest.type) {
        case SPOT_PLANET:
            dx = planets[spotDest.id_planet].x - (currentShip->x + currentShip->w / 2.);
            dy = planets[spotDest.id_planet].y - (currentShip->y + currentShip->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - currentShip->speed >= planets[spotDest.id_planet].radius) {
                currentShip->x += dx * currentShip->speed / distance;
                currentShip->y += dy * currentShip->speed / distance;
            } else {
                currentShip->waitStartTime = currentTime;
                currentShip->state = (currentShip->state == MOVING_TO_BASE) ? WAITING_ON_BASE : WAITING_ON_TARGET;

                // Atterissage des fusees, actualisation de leur angle avec la planete
                if (currentShip->state == WAITING_ON_BASE) {
                    currentShip->angleWithPlanet = computeAngleDeg(currentShip->x + currentShip->w / 2, currentShip->y + currentShip->h / 2, planets[currentShip->base.id_planet].x, planets[currentShip->base.id_planet].y) * M_PI / 180.0;
                } else if (currentShip->state == WAITING_ON_TARGET) {
                    currentShip->angleWithPlanet = computeAngleDeg(currentShip->x + currentShip->w / 2, currentShip->y + currentShip->h / 2, planets[currentShip->target.id_planet].x, planets[currentShip->target.id_planet].y) * M_PI / 180.0;
                } else {
                    currentShip->angleWithPlanet = 0;
                }
            }
            break;
        case SPOT_SHIP:
            dx = ships[spotDest.id_ship].x + ships[spotDest.id_ship].w / 2. - (currentShip->x + currentShip->w / 2.);
            dy = ships[spotDest.id_ship].y + ships[spotDest.id_ship].h / 2. - (currentShip->y + currentShip->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (currentShip->state == ATTACKING_SHIP) {
                //Gestion différente si notre ship est en combat (que ce soit un allié ou enemy peu importe)
                //On s'arrête dès que la cible est à portée
                if (distance >= currentShip->range) {
                    currentShip->x += dx * currentShip->speed / distance;
                    currentShip->y += dy * currentShip->speed / distance;
                }
            }
            else {
                if (distance - currentShip->speed >= (ships[spotDest.id_ship].h + currentShip->h) / 2) {
                    currentShip->x += dx * currentShip->speed / distance;
                    currentShip->y += dy * currentShip->speed / distance;
                } else {
                    currentShip->waitStartTime = currentTime;
                    currentShip->state = (currentShip->state == MOVING_TO_BASE) ? WAITING_ON_BASE : WAITING_ON_TARGET;
                }
            }
            break;
        case SPOT_POINT:
            dx = spotDest.point.x - (currentShip->x + currentShip->w / 2.);
            dy = spotDest.point.y - (currentShip->y + currentShip->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - currentShip->speed > 0) {
                currentShip->x += dx * currentShip->speed / distance;
                currentShip->y += dy * currentShip->speed / distance;
            } else {
                currentShip->waitStartTime = currentTime;
                currentShip->state = (currentShip->state == MOVING_TO_BASE) ? MOVING_TO_TARGET : MOVING_TO_BASE;
            }
            break;
        default:
            break;
    }
}

void updateShipTanks(Ship *ship, Planet *planets, Uint32 currentTime) {  // Gere depot/recuperation des minerais/essence, et consommation essence
    if (currentTime - ship->lastRefreshFilling < TANKS_UPDATE_INTERVAL) {  // Actualisation chaque seconde
        return;
    }

    if (ship->shiptype == ENEMY) {
        return;
    }

    ship->lastRefreshFilling = currentTime;
    if ((ship->state == MOVING_TO_TARGET || ship->state == MOVING_TO_BASE)) {  // Cas ou la fusee est en mouvement
        fuelConsumption(ship);
    } else if (ship->state == WAITING_ON_BASE || ship->state == WAITING_ON_TARGET) {
        if ((ship->state == WAITING_ON_BASE && ship->base.type != SPOT_PLANET) || 
            (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET)) {  // Pas de transfert de ressource ailleurs que sur une planet (actuellement !)
            return;
        }
        OreFillingOrEmptying(ship, planets);
    } else if (ship->state == OUT_OF_FUEL) {
        isShipOnPlanet(ship, planets);
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

void OreFillingOrEmptying(Ship *ship, Planet *planets) {
    if ((ship->state == WAITING_ON_BASE && ship->base.type != SPOT_PLANET) ||
        (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET)) {  // La fusee est-elle sur une planete ?
        return;
    }

    int modified = 0;  // Verifie s'il s'est passe qch (1:oui, 0:non)
    Cargo *cargo = &ship->cargo;
    Planet *landingPlanet = (ship->state == WAITING_ON_BASE) ? &planets[ship->base.id_planet] : &planets[ship->target.id_planet];

    for (int i = 0; i < cargo->compartmentsNumber; i++) {
        if (ship->state == WAITING_ON_BASE) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowBase_in != EMPTY &&  // Eviter ce cas illogique
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_in].tank.currentCapacity > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_in)) {  // La fusee peut recuperer les ressources

                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_in].tank.currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowBase_in].tank.currentCapacity < 0.f) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowBase_in].tank.currentCapacity = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowBase_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.currentCapacity < landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.maxCapacity &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.currentCapacity > landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.maxCapacity) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.currentCapacity = landingPlanet->builds[cargo->compartmentsList[i].flowBase_out].tank.maxCapacity;
                }
                modified = 1;
                break;
            }

            // Changement de ressource
            else if (cargo->compartmentsList[i].currentCapacity == 0.f &&
                     cargo->compartmentsList[i].flowBase_in != EMPTY &&
                     cargo->compartmentsList[i].ore != cargo->compartmentsList[i].flowBase_in) {
                cargo->compartmentsList[i].ore = cargo->compartmentsList[i].flowBase_in;
            }

        } else if (ship->state == WAITING_ON_TARGET) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowTarget_in != EMPTY &&  // Eviter ce cas illogique
                cargo->compartmentsList[i].currentCapacity < cargo->compartmentsList[i].maxCapacity &&  // Les reservoirs d'essence de la fusee sont remplis avant
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in].tank.currentCapacity > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_in)) {  // La fusee peut recuperer les ressources
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in].tank.currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in].tank.currentCapacity < 0.f) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in].tank.currentCapacity = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowTarget_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.currentCapacity < landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.maxCapacity &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.currentCapacity > landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.maxCapacity) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.currentCapacity = landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out].tank.maxCapacity;
                }
                modified = 1;
                break;
            }

            // Changement de ressource
            else if (cargo->compartmentsList[i].currentCapacity == 0.f &&
                     cargo->compartmentsList[i].flowTarget_in != EMPTY &&
                     cargo->compartmentsList[i].ore != cargo->compartmentsList[i].flowTarget_in) {
                cargo->compartmentsList[i].ore = cargo->compartmentsList[i].flowTarget_in;
            }
        }
    }

    // Si les transferts de matieres sont finis, changer l'etat de la fusee
    if (!modified && haveFuel(ship)) {
        ship->state = (ship->state == WAITING_ON_BASE) ? MOVING_TO_TARGET : MOVING_TO_BASE;
    }       
}

int haveFuel(Ship *ship) {
    for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
        if (ship->cargo.compartmentsList[i].ore == FUEL && 
            ship->cargo.compartmentsList[i].currentCapacity > 0) {
            return 1;
        }
    }
    return 0;
}

void isShipOnPlanet(Ship *ship, Planet *planets) {
    if (ship->base.type == SPOT_PLANET) {
        if (distanceShipPlanet(ship, &planets[ship->base.id_planet]) < planets[ship->base.id_planet].radius) {
            ship->state = WAITING_ON_BASE;
        }
    } else if (ship->target.type == SPOT_PLANET) {
        if (distanceShipPlanet(ship, &planets[ship->target.id_planet]) < planets[ship->target.id_planet].radius) {
            ship->state = WAITING_ON_TARGET;
        }
    }
}

void displayShips(SDL_Texture ***imageTextures, Ship *ships, int shipCount, Planet *planets) {
    for (int i = 0; i < shipCount; i++) {
        // Calcul des coordonnees a l'ecran, du point en haut a gauche de la fusee
        SDL_Point ShipOnScreen = (SDL_Point){(ships[i].x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                             (ships[i].y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};

        if (ShipOnScreen.x >= -ships[i].w * getCameraScale() && ShipOnScreen.x <= SCREEN_WIDTH && 
            ShipOnScreen.y >= -ships[i].h * getCameraScale() && ShipOnScreen.y <= SCREEN_HEIGHT + ships[i].h * getCameraScale()) {  // Si la fusee est dans l'ecran 
            renderShipImage(imageTextures[7][ships[i].idModel], ships, planets, ships[i], ShipOnScreen);
            // renderShipBars(ships[i], ShipOnScreen);
            ships[i].destRect.x = ShipOnScreen.x;
            ships[i].destRect.y = ShipOnScreen.y;
            ships[i].destRect.w = ships[i].w * getCameraScale();
            ships[i].destRect.h = ships[i].h * getCameraScale();
        }
    }
}

void renderShipImage(SDL_Texture *textureShip, Ship *ships, Planet *planets, Ship currentShip, SDL_Point ShipOnScreen) {
    // Calcul de l'angle en degres de l'image de la fusee
    float angle;
    Spot spotDest = (currentShip.state == WAITING_ON_BASE || currentShip.state == MOVING_TO_BASE) ? currentShip.base : currentShip.target;

    switch (spotDest.type) {
        case SPOT_PLANET:
            angle = atan2(planets[spotDest.id_planet].y - (currentShip.y + currentShip.h / 2.f), planets[spotDest.id_planet].x - (currentShip.x + currentShip.w / 2.f)) * 180.0f / M_PI;
            break;
        case SPOT_SHIP:
            angle = atan2((ships[spotDest.id_ship].y + ships[spotDest.id_ship].h / 2.f) - (currentShip.y + currentShip.h / 2.f), (ships[spotDest.id_ship].x + ships[spotDest.id_ship].w) - (currentShip.x + currentShip.w / 2.f)) * 180.0f / M_PI;
            if (currentShip.state == ATTACKING_SHIP) {
                //Eviter que les ships n'attaquent en arrière
                angle += 180;
            }
            break;
        case SPOT_POINT:
            angle = atan2(spotDest.point.y - (currentShip.y + currentShip.h / 2.f), spotDest.point.x - (currentShip.x + currentShip.w / 2.f)) * 180.0f / M_PI;
            break;
        default:
            angle = 0;
            break;
    }

    // Ajuste l'angle en fonction du sens de deplacement
    angle +=(currentShip.state == MOVING_TO_BASE || currentShip.state == MOVING_TO_TARGET) ? 90 : -90;

    // Creation des variables necessaires a l'affichage
    SDL_Rect srcRect = {currentShip.frameIndex * 64, 0, 64, 64};  // Frame actuelle sur le sprite sheet
    SDL_Rect destRect = {ShipOnScreen.x, ShipOnScreen.y, currentShip.w * getCameraScale(), currentShip.h * getCameraScale()};  // Position et taille affichee
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
    free(ships);
}

int globalTankCapacity(Ship *ship) {
    int capacity = 0;

    for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
        capacity += ship->cargo.compartmentsList[i].maxCapacity;
    }

    return capacity;
}
