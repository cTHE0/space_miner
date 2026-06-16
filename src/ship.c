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
#include "window.h"
#include "notify.h"
#include "assets_gestion.h"
#include "meta.h"


static uint32_t lastRefreshShipKilled = 0;


void initShips(Ship **ships, int shipCount, Planet *planets) {
    *ships = malloc(shipCount * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation memoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour eviter d'utiliser ships apres un echec
    }

    for (int i = 0; i < shipCount; i++) {
        memset(&(*ships)[i], 0, sizeof(Ship));
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
        (*ships)[i].maxLife = 100 + shipArmorBonus();
        (*ships)[i].currentLife = (*ships)[i].maxLife;  // Les fusees demarrent en pleine sante
        (*ships)[i].fuelConsumption = 1 + rand() % 3;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
        (*ships)[i].range = 300;
        (*ships)[i].noise = 1 + rand() % 3;
        (*ships)[i].transferredMinerals = 0;

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
        updateShipMove(ships, &ships[i], planets);
        updateShipTanks(&ships[i], planets, currentTime);
    }
}

void updateShipAnimation(Ship *ship, Uint32 currentTime) {  // Pour animation de la flamme des fusees 
    if (currentTime > ship->lastFrameTime + SPRITE_SHEETS_DELAY) {
        ship->frameIndex = (ship->frameIndex + 1) % 4;  // 4 images dans le sprite sheet
        ship->lastFrameTime = currentTime;
    }
}

void updateShipMove(Ship *ships, Ship *currentShip, Planet *planets) {
    if (currentShip->state != MOVING_TO_BASE && currentShip->state != MOVING_TO_TARGET && 
        currentShip->state != MOVING_TO_BASE_SOON_STOPPED && currentShip->state != MOVING_TO_TARGET_SOON_STOPPED && 
        currentShip->state !=ATTACKING_SHIP) { // La fusee bouge-t-elle ?
        return;
    }

    if (currentShip->base.type == SPOT_PLANET && currentShip->target.type == SPOT_PLANET && 
        currentShip->base.id_planet == currentShip->target.id_planet) {
        if (currentShip->state == MOVING_TO_BASE)
            currentShip->state = MOVING_TO_BASE_SOON_STOPPED;
        else if (currentShip->state == MOVING_TO_TARGET)
            currentShip->state = MOVING_TO_TARGET_SOON_STOPPED;
    }

    float dx;
    float dy;
    float distance;

    Spot spotDest;
    if (currentShip->state == MOVING_TO_BASE || currentShip->state == MOVING_TO_BASE_SOON_STOPPED) {
        spotDest = currentShip->base;
    } else {  // voir la condition implicite de debut de fonction 'updateShipMove'
        spotDest = currentShip->target;
    }

    switch (spotDest.type) {
        case SPOT_PLANET:
            dx = planets[spotDest.id_planet].x - (currentShip->x + currentShip->w / 2.);
            dy = planets[spotDest.id_planet].y - (currentShip->y + currentShip->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (distance - currentShip->speed >= planets[spotDest.id_planet].radius) {
                currentShip->x += dx * currentShip->speed / distance;
                currentShip->y += dy * currentShip->speed / distance;
            } else {
                // Mise a jour de l'etat de la fusee
                if (currentShip->state == MOVING_TO_BASE) {
                    currentShip->state = WAITING_ON_BASE;
                } else if (currentShip->state == MOVING_TO_TARGET) {
                    currentShip->state = WAITING_ON_TARGET;
                } else if (currentShip->state == MOVING_TO_BASE_SOON_STOPPED) {
                    currentShip->state = WAITING_ON_BASE_SOON_STOPPED;
                } else if (currentShip->state == MOVING_TO_TARGET_SOON_STOPPED) {
                    currentShip->state = WAITING_ON_TARGET_SOON_STOPPED;
                }

                // Atterissage des fusees, actualisation de leur angle avec la planete
                if (currentShip->state == WAITING_ON_BASE || currentShip->state == WAITING_ON_BASE_SOON_STOPPED) {
                    currentShip->angleWithPlanet = computeAngleDeg(currentShip->x + currentShip->w / 2, currentShip->y + currentShip->h / 2, planets[currentShip->base.id_planet].x, planets[currentShip->base.id_planet].y) * M_PI / 180.0;
                } else if (currentShip->state == WAITING_ON_TARGET || currentShip->state == WAITING_ON_TARGET_SOON_STOPPED) {
                    currentShip->angleWithPlanet = computeAngleDeg(currentShip->x + currentShip->w / 2, currentShip->y + currentShip->h / 2, planets[currentShip->target.id_planet].x, planets[currentShip->target.id_planet].y) * M_PI / 180.0;
                } else {
                    currentShip->angleWithPlanet = 0;
                }
            }
            break;
        case SPOT_SHIP:  // Reserve pour les fusees offensives, plus du tout pour les transporteurs (inutile !)
            dx = ships[spotDest.id_ship].x + ships[spotDest.id_ship].w / 2. - (currentShip->x + currentShip->w / 2.);
            dy = ships[spotDest.id_ship].y + ships[spotDest.id_ship].h / 2. - (currentShip->y + currentShip->h / 2.);
            distance = sqrt(dx * dx + dy * dy);

            if (currentShip->state == ATTACKING_SHIP) {
                // Gestion différente si notre ship est en combat (que ce soit un allié ou enemy peu importe)
                // On s'arrête dès que la cible est à portée
                if (distance >= currentShip->range) {
                    currentShip->x += dx * currentShip->speed / distance;
                    currentShip->y += dy * currentShip->speed / distance;
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
                // Mise a jour de l'etat de la fusee
                if (currentShip->state == MOVING_TO_BASE) {
                    currentShip->state = MOVING_TO_TARGET;
                } else if (currentShip->state == MOVING_TO_TARGET) {
                    currentShip->state = MOVING_TO_BASE;
                } else if (currentShip->state == MOVING_TO_BASE_SOON_STOPPED) {
                    currentShip->state = STOPPED_ON_BASE;
                } else if (currentShip->state == MOVING_TO_TARGET_SOON_STOPPED) {
                    currentShip->state = STOPPED_ON_TARGET;
                }
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

    if (ship->shiptype == ENEMY || ship->shiptype == DEFENDER) {
        return;  // Ni les pirates ni les defenseurs n'utilisent de cargo/essence
    }

    ship->lastRefreshFilling = currentTime;
    if (ship->state == MOVING_TO_TARGET || ship->state == MOVING_TO_BASE || 
        ship->state == MOVING_TO_TARGET_SOON_STOPPED || ship->state == MOVING_TO_BASE_SOON_STOPPED) {  // Cas ou la fusee est en mouvement
        fuelConsumption(ship);
    } else if (ship->state == WAITING_ON_BASE || ship->state == WAITING_ON_TARGET ||
               ship->state == WAITING_ON_BASE_SOON_STOPPED || ship->state == WAITING_ON_TARGET_SOON_STOPPED) {
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
        (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET) ||
        (ship->state == WAITING_ON_BASE_SOON_STOPPED && ship->base.type != SPOT_PLANET) ||
        (ship->state == WAITING_ON_TARGET_SOON_STOPPED && ship->target.type != SPOT_PLANET)) {  // Pas de transfert de ressource ailleurs que sur une planet (actuellement !)
        return;
    }

    int modified = 0;  // Verifie s'il s'est passe qch (1:oui, 0:non)
    Cargo *cargo = &ship->cargo;
    Planet *landingPlanet = (ship->state == WAITING_ON_BASE || ship->state == WAITING_ON_BASE_SOON_STOPPED) ? &planets[ship->base.id_planet] : &planets[ship->target.id_planet];

    for (int i = 0; i < cargo->compartmentsNumber; i++) {
        if ((ship->cargo.compartmentsList[i].flowBase_in != EMPTY && ship->cargo.compartmentsList[i].flowBase_in == ship->cargo.compartmentsList[i].flowBase_out) ||
            (ship->cargo.compartmentsList[i].flowTarget_in != EMPTY && ship->cargo.compartmentsList[i].flowTarget_in == ship->cargo.compartmentsList[i].flowTarget_out)) {  // Si le joueur fait le coquin
            continue;
        }
        if (ship->state == WAITING_ON_BASE || ship->state == WAITING_ON_BASE_SOON_STOPPED) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowBase_in != EMPTY &&  // Eviter ce cas illogique
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_in * 2 + 2].tank.currentCapacity > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_in) &&
                cargo->compartmentsList[i].currentCapacity < cargo->compartmentsList[i].maxCapacity) {  // La fusee peut recuperer les ressources

                // Si le conteneur est vide, lui assigner un minerai
                if (cargo->compartmentsList[i].ore == EMPTY) {
                    cargo->compartmentsList[i].ore = cargo->compartmentsList[i].flowBase_in;
                }

                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                ship->transferredMinerals += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_in * 2 + 2].tank.currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowBase_in * 2 + 2].tank.currentCapacity < 0.f) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowBase_in * 2 + 2].tank.currentCapacity = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowBase_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.currentCapacity < landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.maxCapacity &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowBase_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.currentCapacity > landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.maxCapacity) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.currentCapacity = landingPlanet->builds[cargo->compartmentsList[i].flowBase_out * 2 + 2].tank.maxCapacity;
                }

                // Si le conteneur vient d'etre vide
                if (cargo->compartmentsList[i].ore != EMPTY && cargo->compartmentsList[i].currentCapacity == 0) {
                    cargo->compartmentsList[i].ore = EMPTY;
                }

                modified = 1;
                break;
            }

        } else if (ship->state == WAITING_ON_TARGET || ship->state == WAITING_ON_TARGET_SOON_STOPPED) {
            // Remplissage de la fusee
            if (cargo->compartmentsList[i].flowTarget_in != EMPTY &&  // Eviter ce cas illogique
                cargo->compartmentsList[i].currentCapacity < cargo->compartmentsList[i].maxCapacity &&  // Les reservoirs d'essence de la fusee sont remplis avant
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in * 2 + 2].tank.currentCapacity > 0.f &&  // La planete a des stocks
                (cargo->compartmentsList[i].ore == EMPTY || cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_in) &&
                cargo->compartmentsList[i].currentCapacity < cargo->compartmentsList[i].maxCapacity) {  // La fusee peut recuperer les ressources
                
                // Si le conteneur est vide, lui assigner un minerai
                if (cargo->compartmentsList[i].ore == EMPTY) {
                    cargo->compartmentsList[i].ore = cargo->compartmentsList[i].flowTarget_in;
                }
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity += cargo->compartmentsList[i].flowSpeed;
                ship->transferredMinerals += cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity > cargo->compartmentsList[i].maxCapacity) {
                    cargo->compartmentsList[i].currentCapacity = cargo->compartmentsList[i].maxCapacity;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in * 2 + 2].tank.currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in * 2 + 2].tank.currentCapacity < 0.f) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowTarget_in * 2 + 2].tank.currentCapacity = 0.f;
                }
                modified = 1;
                break;
            }

             // Vidange de la fusee
            else if (cargo->compartmentsList[i].flowTarget_out != EMPTY &&  // Eviter ce cas illogique
                     landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.currentCapacity < landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.maxCapacity &&  // La planete a de la place
                     cargo->compartmentsList[i].currentCapacity > 0.f &&  // La fusee a des ressources a deposer
                     cargo->compartmentsList[i].ore == cargo->compartmentsList[i].flowTarget_out) {  // Le compartiment a le bon contenu
                
                // Actualisation fusee
                cargo->compartmentsList[i].currentCapacity -= cargo->compartmentsList[i].flowSpeed;
                if (cargo->compartmentsList[i].currentCapacity < 0) {
                    cargo->compartmentsList[i].currentCapacity = 0;
                }

                // Actualisation planete
                landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.currentCapacity += cargo->compartmentsList[i].flowSpeed;
                if (landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.currentCapacity > landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.maxCapacity) {
                    landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.currentCapacity = landingPlanet->builds[cargo->compartmentsList[i].flowTarget_out * 2 + 2].tank.maxCapacity;
                }

                // Si le conteneur vient d'etre vide
                if (cargo->compartmentsList[i].ore != EMPTY && cargo->compartmentsList[i].currentCapacity == 0) {
                    cargo->compartmentsList[i].ore = EMPTY;
                }

                modified = 1;
                break;
            }
        }
    }

    // Si les transferts de matieres sont finis, changer l'etat de la fusee
    if (!modified && haveFuel(ship)) {
        if (ship->state == WAITING_ON_BASE) {
            ship->state = MOVING_TO_TARGET;
        } else if (ship->state == WAITING_ON_TARGET) {
            ship->state = MOVING_TO_BASE;
        } else if (ship->state == WAITING_ON_BASE_SOON_STOPPED) {
            ship->state = STOPPED_ON_BASE;
        } else if (ship->state == WAITING_ON_TARGET_SOON_STOPPED) {
            ship->state = STOPPED_ON_TARGET;
        }
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
        // Actualisation des coordonnees a l'ecran de la fusee
        ships[i].destRect = (SDL_Rect){(ships[i].x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                       (ships[i].y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f,
                                       ships[i].w * getCameraScale(),
                                       ships[i].h * getCameraScale()};

        if (ships[i].destRect.x >= -ships[i].destRect.w && ships[i].destRect.x <= SCREEN_WIDTH &&
            ships[i].destRect.y >= -ships[i].destRect.h && ships[i].destRect.y <= SCREEN_HEIGHT + ships[i].destRect.h) {  // Si la fusee est dans l'ecran
            SDL_Point center = {ships[i].destRect.w / 2, ships[i].destRect.h / 2};  // Definition du point de rotation (au centre du sprite)

            if (ships[i].shiptype == ENEMY) {
                // Affichage d'un pirate (sprite d'attaquant)
                SDL_RenderCopyEx(renderer,
                                imageTextures[1][0],
                                NULL,
                                &ships[i].destRect,
                                angleShipImage(ships, planets, &ships[i]) + 90,
                                &center,
                                SDL_FLIP_NONE);
            } else {
                // Affichage d'une fusee alliee
                SDL_Rect srcRect = {ships[i].frameIndex * 64, 0, 64, 64};  // Frame actuelle sur le sprite sheet
                SDL_RenderCopyEx(renderer,
                                imageTextures[7][ships[i].idModel],
                                &srcRect,
                                &ships[i].destRect,
                                angleShipImage(ships, planets, &ships[i]),
                                &center,
                                SDL_FLIP_NONE);
            }

            // Barre de vie affichee lorsque le vaisseau est endommage
            if (ships[i].currentLife < ships[i].maxLife) {
                SDL_Rect lifeBg = {ships[i].destRect.x, ships[i].destRect.y - 8, ships[i].destRect.w, 5};
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 220);
                SDL_RenderFillRect(renderer, &lifeBg);

                SDL_Rect lifeFg = lifeBg;
                lifeFg.w = lifeBg.w * ships[i].currentLife / (float)ships[i].maxLife;
                if (ships[i].shiptype == ENEMY) {
                    SDL_SetRenderDrawColor(renderer, 230, 60, 60, 255);  // Rouge pour les ennemis
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);   // Vert pour les allies
                }
                SDL_RenderFillRect(renderer, &lifeFg);
            }
        }
    }
}

float angleShipImage(Ship *ships, Planet *planets, Ship *currentShip) {  // Calcul de l'angle en degres de l'image de la fusee
    float angle;
    Spot spotDest = (currentShip->state == WAITING_ON_BASE || currentShip->state == MOVING_TO_BASE || currentShip->state == WAITING_ON_BASE_SOON_STOPPED || currentShip->state == MOVING_TO_BASE_SOON_STOPPED || currentShip->state == STOPPED_ON_BASE) ? currentShip->base : currentShip->target;

    switch (spotDest.type) {
        case SPOT_PLANET:
            angle = atan2(planets[spotDest.id_planet].y - (currentShip->y + currentShip->h / 2.f), planets[spotDest.id_planet].x - (currentShip->x + currentShip->w / 2.f)) * 180.0f / M_PI;
            break;
        case SPOT_SHIP:
            angle = atan2((ships[spotDest.id_ship].y + ships[spotDest.id_ship].h / 2.f) - (currentShip->y + currentShip->h / 2.f), (ships[spotDest.id_ship].x + ships[spotDest.id_ship].w) - (currentShip->x + currentShip->w / 2.f)) * 180.0f / M_PI;
            if (currentShip->state == ATTACKING_SHIP) {
                //Eviter que les ships n'attaquent en arrière
                angle += 180;
            }
            break;
        case SPOT_POINT:
            angle = atan2(spotDest.point.y - (currentShip->y + currentShip->h / 2.f), spotDest.point.x - (currentShip->x + currentShip->w / 2.f)) * 180.0f / M_PI;
            break;
        default:
            angle = 0;
            break;
    }

    // Ajuste l'angle en fonction du sens de deplacement
    angle +=(currentShip->state == MOVING_TO_BASE || currentShip->state == MOVING_TO_TARGET || currentShip->state == MOVING_TO_BASE_SOON_STOPPED || currentShip->state == MOVING_TO_TARGET_SOON_STOPPED) ? 90 : -90;

    return angle;
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

int fuelInShip(Ship *ship) {
    int totalFuel = 0;
    for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
        if (ship->cargo.compartmentsList[i].ore == FUEL) {
            totalFuel += ship->cargo.compartmentsList[i].currentCapacity;
        }
    }
    return totalFuel;
}

void addDefender(Ship **ships, int *shipCount, Planet *planet) {
    Ship *temp = realloc(*ships, (*shipCount + 1) * sizeof(Ship));
    if (temp == NULL) return;
    *ships = temp;

    Ship *d = &(*ships)[*shipCount];
    memset(d, 0, sizeof(Ship));

    d->shiptype = DEFENDER;
    d->id = *shipCount;
    d->idModel = 0;  // Sprite distinct des transporteurs
    d->level = 1;

    d->x = planet->x;
    d->y = planet->y;
    d->w = 170;
    d->h = 170;
    d->speed = (0.8f + (rand() % 40) / 100.f) * SHIP_SPEED * shipSpeedMultiplier();
    d->maxLife = 140 + shipArmorBonus();
    d->currentLife = d->maxLife;
    d->fuelConsumption = 1;
    d->range = 2600;  // Surclasse la portee des pirates

    d->state = WAITING_ON_BASE;
    d->base.type = SPOT_PLANET;
    d->base.id_planet = planet->id;
    d->target.type = SPOT_PLANET;
    d->target.id_planet = planet->id;

    d->frameIndex = rand() % 4;
    d->lastRefreshFiring = SDL_GetTicks();

    (*shipCount)++;

    pushNotification("Defender deployed!", GREEN);
}

void addShip(Ship **ships, int *shipCount, Planet *planet, int planetCount) {
    *ships = realloc(*ships, (*shipCount + 1) * sizeof(Ship));
    if (*ships == NULL) {
        exit(1);
    }

    memset(&(*ships)[*shipCount], 0, sizeof(Ship));

    (*ships)[*shipCount].shiptype = TRANSPORTER;
    (*ships)[*shipCount].id = *shipCount;
    (*ships)[*shipCount].idModel = 6;
    (*ships)[*shipCount].state = WAITING_ON_BASE;

    (*ships)[*shipCount].base.type = SPOT_PLANET;
    (*ships)[*shipCount].base.id_planet = planet->id;
    (*ships)[*shipCount].target.type = SPOT_PLANET;
    (*ships)[*shipCount].target.id_planet = (planetCount > 1) ? (planet->id + 1) % planetCount : planet->id;

    (*ships)[*shipCount].noise = 1;  // Eviter une division par zero lors de la generation d'ennemis
    (*ships)[*shipCount].transferredMinerals = 0;
    (*ships)[*shipCount].angleWithPlanet = 0;

    (*ships)[*shipCount].x = planet->x;
    (*ships)[*shipCount].y = planet->y;
    (*ships)[*shipCount].w = 200;
    (*ships)[*shipCount].h = 200;
    (*ships)[*shipCount].speed = (rand() / (float)RAND_MAX * 0.6 + 0.4) * SHIP_SPEED * shipSpeedMultiplier();
    (*ships)[*shipCount].level = 1;

    (*ships)[*shipCount].maxLife = 100 + shipArmorBonus();
    (*ships)[*shipCount].currentLife = (*ships)[*shipCount].maxLife;
    (*ships)[*shipCount].fuelConsumption = 1;  // Consommation d'essence par intervalle de temps TANKS_UPDATE_INTERVAL
    (*ships)[*shipCount].range = 1000;

    (*ships)[*shipCount].frameIndex = rand() % 4;  // Desynchronisation des fusees
    (*ships)[*shipCount].lastFrameTime = 0;
    (*ships)[*shipCount].lastRefreshFilling = SDL_GetTicks();

    // Allocation des compartiments
    Cargo *cargo = &(*ships)[*shipCount].cargo;

    cargo->compartmentsNumber = 2;  // Un premier compartiment rempli d'essence, le second vide
    cargo->compartmentsList[0].ore = 0;
    cargo->compartmentsList[0].maxCapacity = 1000;
    cargo->compartmentsList[0].currentCapacity = 1000;
    cargo->compartmentsList[0].flowSpeed = 20;
    cargo->compartmentsList[0].level = 1;
    cargo->compartmentsList[0].flowBase_in = FUEL;
    cargo->compartmentsList[0].flowBase_out = EMPTY;
    cargo->compartmentsList[0].flowTarget_in = FUEL;
    cargo->compartmentsList[0].flowTarget_out = EMPTY;

    cargo->compartmentsList[1].ore = EMPTY;
    cargo->compartmentsList[1].maxCapacity = 1000;
    cargo->compartmentsList[1].currentCapacity = 0;
    cargo->compartmentsList[1].flowSpeed = 20;
    cargo->compartmentsList[1].level = 0;
    cargo->compartmentsList[1].flowBase_in = EMPTY;
    cargo->compartmentsList[1].flowBase_out = EMPTY;
    cargo->compartmentsList[1].flowTarget_in = EMPTY;
    cargo->compartmentsList[1].flowTarget_out = EMPTY;

    (*shipCount) ++;

    getMeta()->shipsBuilt++;
    pushNotification("New transporter ready!", GREEN);
}

void deleteShip(int index, Ship **ships, int *shipCount) {
    // Arret du suivi de la camera si c'est necessaire
    if (getCameraMode() == FOLLOW_SHIP && getCameraLastObjectSelected() == index) {
        setCameraMode(NORMAL);
        setCenterCamera((SDL_Point){(*ships)[index].x, (*ships)[index].y});
    }

    // Fermeture de la page d'information d'une fusee qui vient d'etre detruite
    if ((getWindowType() == SHIP_WINDOW || getWindowType() == BASIC_SHIP_WINDOW) && getWindowId() == index) {
        setWindowType(NO_WINDOW);
    }

    // Liste des indices à supprimer
    int toDelete[256];
    int deleteCount = 0;

    // Initialisation de la liste des fusees a supprimer
    toDelete[0] = index;
    deleteCount ++;

    // Si on supprime un allié, les ennemis peuvent réagir
    if ((*ships)[index].shiptype != ENEMY) {
        for (int i = 0; i < *shipCount; i++) {
            if ((*ships)[i].shiptype == ENEMY && (*ships)[i].target.id_ship == index) {  // Recherche des ennemis qui tiraient sur index
                int idNewTarget = newTarget(*ships, *shipCount, i, index);
                if (idNewTarget != -1) {
                    (*ships)[i].target.id_ship = idNewTarget;
                } else {
                    toDelete[deleteCount] = i;  // Ennemis sans nouvelle cible
                    deleteCount++;
                }
            }
        }
    }

    // Suppression des references vers les fusees qui vont etre supprimees
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].base.type == SPOT_SHIP) {
            for (int j = 0; j < deleteCount; j++) {
                if ((*ships)[i].base.id_ship == toDelete[j]) {
                    (*ships)[i].base.type = SPOT_NONE;
                    break;
                }
            }
        }
        if ((*ships)[i].target.type == SPOT_SHIP) {
            for (int j = 0; j < deleteCount; j++) {
                if ((*ships)[i].target.id_ship == toDelete[j]) {
                    (*ships)[i].target.type = SPOT_NONE;
                    break;
                }
            }
        }
    }

    // Suppression des references que les lasers ont sur les fusees qui vont etre supprimees
    for (int i = 0; i < getLasersCount(); i++) {
        for (int j = 0; j < deleteCount; j++) {
            if (getLasersList()[i].target_id == toDelete[j]) {
                getLasersList()[i].target_id = -1;
                break;
            }
        }
    }

    // Suppression des elements de 'toDelete' et decalage des indices, avec actualisation des references entre fusees
    for (int i = 0; i < deleteCount; i++) {
        // Remplissage du trou (ce qui supprime bien lelement d'indice toDelete[i])
        if (toDelete[i] != *shipCount - 1) {
            (*ships)[toDelete[i]] = (*ships)[*shipCount - 1];
        }

        // Recherche des fusees qui avaient un lien avec la ship 'shipCount - 1' et actualisation
        for (int j = 0; j < *shipCount; j++) {
            if ((*ships)[j].base.type == SPOT_SHIP && (*ships)[j].base.id_ship == *shipCount - 1) {
                (*ships)[j].base.id_ship = toDelete[i];
            }
            if ((*ships)[j].target.type == SPOT_SHIP && (*ships)[j].target.id_ship == *shipCount - 1) {
                (*ships)[j].target.id_ship = toDelete[i];
            }
        }

        // Recherche du cas ou (*ships)[*shipCount - 1] est un element de toDelete
        for (int j = i + 1; j < deleteCount; j++) {
            if (toDelete[j] == *shipCount - 1) {
                toDelete[j] = toDelete[i];
            }
        }

        (*shipCount) --;
    }

    // Reallocation de la liste de fusees
    if (*shipCount != 0) {
        *ships = realloc(*ships, (*shipCount) * sizeof(Ship));
    }
}

void deleteKilledShips(Ship **ships, int *shipCount) {  // On tue tous les ships qui n'ont plus de vie
    if (SDL_GetTicks() < lastRefreshShipKilled + REFRESH_TIME_KILLED_SHIP) return;

    lastRefreshShipKilled = SDL_GetTicks();
    for (int i = 0; i < *shipCount; i++) {
        if ((*ships)[i].currentLife <= 0) {
            if ((*ships)[i].shiptype == ENEMY) {
                registerPirateKill((*ships)[i].maxLife);  // Prime + score (selon robustesse)
                if ((*ships)[i].maxLife >= ENEMY_LIFE * 5) {
                    pushNotification("Pirate dreadnought destroyed!", GOLD);
                } else {
                    pushNotification("Pirate destroyed!", GREEN);
                }
            } else {
                pushNotification("A transporter was lost!", RED);
            }
            deleteShip(i, ships, shipCount);
        }
    }
}

int globalTankCapacity(Ship *ship) {
    int capacity = 0;

    for (int i = 0; i < ship->cargo.compartmentsNumber; i++) {
        capacity += ship->cargo.compartmentsList[i].maxCapacity;
    }

    return capacity;
}

void destroyShips(Ship *ships) {
    free(ships);
}
