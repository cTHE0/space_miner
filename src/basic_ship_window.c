#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"
#include "config.h"
#include "camera.h"
#include "event.h"


static BasicShipWindowButton buttonSelected = NO_BUTTON; // Lequel des 3 boutons est sélectionné ? AU début, aucun des boutons n'est sélectionné

static SDL_Rect bgRect = {
                            SCREEN_WIDTH/12, 
                            3*SCREEN_HEIGHT/4, 
                            5*SCREEN_WIDTH/6, 
                            SCREEN_HEIGHT/4
                        };

static SDL_Rect shipPictureRect = {
                                    SCREEN_WIDTH/12 + SCREEN_HEIGHT/32, 
                                    3*SCREEN_HEIGHT/4, 
                                    SCREEN_HEIGHT/4, 
                                    SCREEN_HEIGHT/4
                                };

static SDL_Rect leftArrowRect = {
                                    SCREEN_WIDTH/12, 
                                    7*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/64, 
                                    SCREEN_HEIGHT/32, 
                                    SCREEN_HEIGHT/32
                                };

static SDL_Rect rightArrowRect = {
                                    11*SCREEN_WIDTH/12 - SCREEN_HEIGHT/32, 
                                    7*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/64, 
                                    SCREEN_HEIGHT/32, 
                                    SCREEN_HEIGHT/32
                                };

static SDL_Rect line1Rect = {
                                SCREEN_WIDTH/12 + SCREEN_HEIGHT/32 + SCREEN_HEIGHT/4, 
                                3*SCREEN_HEIGHT/4 + SCREEN_HEIGHT/32, 
                                SCREEN_WIDTH/12 + 5*SCREEN_WIDTH/6 - (SCREEN_WIDTH/12 + SCREEN_HEIGHT/32 + SCREEN_HEIGHT/4), 
                                3
                            };

static SDL_Rect crossRect = {
                                11*SCREEN_WIDTH/12 - 3*SCREEN_HEIGHT/128, 
                                3*SCREEN_HEIGHT/4 + SCREEN_HEIGHT/128, 
                                SCREEN_HEIGHT/64, 
                                SCREEN_HEIGHT/64
                            };

static SDL_Rect button1Rect = {
                                    SCREEN_WIDTH/2 - SCREEN_HEIGHT/8, 
                                    3*SCREEN_HEIGHT/4 - SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/32
                                };
                        
static SDL_Rect button2Rect = {
                                    SCREEN_WIDTH/2 - SCREEN_HEIGHT/32, 
                                    3*SCREEN_HEIGHT/4 - SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/32
                                };

static SDL_Rect button3Rect = {
                                    SCREEN_WIDTH/2 + SCREEN_HEIGHT/16, 
                                    3*SCREEN_HEIGHT/4 - SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/16, 
                                    SCREEN_HEIGHT/32
                                };


void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships){
    if (getWindowType() != BASIC_SHIP_WINDOW) {  // La fenetre d'informations basiques d'une fusee est-elle ouverte ?
        return;
    }

    // Afficher la portee de la fusee
    SDL_Rect rangeCircle = (SDL_Rect){ships[getWindowId()].destRect.x - getCameraScale() * ships[getWindowId()].range, 
                             ships[getWindowId()].destRect.y - getCameraScale() * ships[getWindowId()].range, 
                             2 * getCameraScale() * ships[getWindowId()].range, 
                             2 * getCameraScale() * ships[getWindowId()].range};
    SDL_RenderCopy(renderer, imageTextures[5][4], NULL, &rangeCircle);

    // Afficher fenetre du bas
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);                                       //Afficher fond
    SDL_RenderCopy(renderer, imageTextures[8][0], NULL, &shipPictureRect);                              //Afficher "photo" du ship
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &leftArrowRect);                                //Afficher flèche gauche
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &rightArrowRect, 180, NULL, SDL_FLIP_NONE);   // Afficher flèche droite
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &line1Rect);                                                           // Ligne horizontale sous nom fusée

    // Affichage boutons au-dessus fenêtre
    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button1Rect);
    SDL_RenderCopy(renderer, imageTextures[2][4], NULL, &button1Rect);

    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button2Rect);

    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button3Rect);

    // Afficher la ligne en pointillees 
    if (buttonSelected == MOVING_BUTTON) {
        SDL_Point centerShipCoord = {ships[getWindowId()].destRect.x + ships[getWindowId()].destRect.w / 2,
                                     ships[getWindowId()].destRect.y + ships[getWindowId()].destRect.h / 2};
        plotPath(centerShipCoord, getMouseCoordinates(), 10, 5);
    }
}

int changeButtonType(SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &button1Rect)) {
        buttonSelected = (buttonSelected == MOVING_BUTTON) ? NO_BUTTON : MOVING_BUTTON;
        return 1;
    }
    return 0;
}

void plotPath(SDL_Point origin, SDL_Point destination, int dashLength, int gapLength){
    int x1 = origin.x, y1 = origin.y, x2 = destination.x, y2 = destination.y;
    float deltaX = x2 - x1;
    float deltaY = y2 - y1;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    float dashAndGap = dashLength + gapLength;
    float nbDashes = distance / dashAndGap;

    float unitX = deltaX / distance;
    float unitY = deltaY / distance;

    for (int i = 0; i < nbDashes; i++) {
        float startX = x1 + (i * dashAndGap) * unitX;
        float startY = y1 + (i * dashAndGap) * unitY;
        float endX = startX + dashLength * unitX;
        float endY = startY + dashLength * unitY;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, (int)startX, (int)startY, (int)endX, (int)endY);
    }  
}

void choosingNewTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse){
    if (buttonSelected != MOVING_BUTTON) {  // Sommes-nous en mode deplacement. Si oui, continuer.
        return;
    }

    int planetChosen = whichPlanetIsClicked(planets, planetCount, mouse);
    int shipChosen = whichShipIsClicked(ships, shipCount, mouse);

    if (planetChosen != -1) {  // La nouvelle cible est une planète
        ships[getWindowId()].target.type = TARGET_PLANET;
        ships[getWindowId()].target.planet = &planets[planetChosen];
    } else if (shipChosen != -1) {  // La nouvelle cible est un ship (par ex. une station spatiale, orbitale ou un vaisseau de ravitaillement)
        ships[getWindowId()].target.type = TARGET_SHIP;
        ships[getWindowId()].target.ship = &ships[shipChosen];
    } else {  // La nouvelle target est un point random de l'espace
        ships[getWindowId()].target.type = TARGET_POINT;
        ships[getWindowId()].target.point = (SDL_Point){mouse.x, mouse.y};
    }
}
