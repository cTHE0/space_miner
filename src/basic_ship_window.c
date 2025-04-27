#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"
#include "config.h"
#include "camera.h"
#include "event.h"

SDL_Rect rangeCircle;
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

SDL_Rect pathRect;
int idShip; //identifiant du ship dont la basic window est à afficher
float cameraScale;

void initBasicShipWindow(int i){
    idShip = i;
}

BasicShipWindowButton buttonSelected = NO_BUTTON; // Lequel des 3 boutons est sélectionné ? AU début, aucun des boutons n'est sélectionné

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships){
    if (getWindowType() != BASIC_SHIP_WINDOW) {  // La fenetre d'informations [basiques] d'une fusee est-elle ouverte ?
        return;
    }
    SDL_Rect shipDestRect = getShipOnScreen(ships[idShip]);
    cameraScale = getCameraScale();

    rangeCircle = (SDL_Rect){shipDestRect.x - cameraScale * ships[idShip].range, shipDestRect.y - cameraScale * ships[idShip].range, 2*cameraScale*ships[idShip].range, 2*cameraScale*ships[idShip].range};
    SDL_RenderCopy(renderer, imageTextures[5][4], NULL, &rangeCircle);

    //Afficher fenetre du bas
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);                                       //Afficher fond
    SDL_RenderCopy(renderer, imageTextures[8][0], NULL, &shipPictureRect);                              //Afficher "photo" du ship
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &leftArrowRect);                                //Afficher flèche gauche
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &rightArrowRect, 180, NULL, SDL_FLIP_NONE);   // Afficher flèche droite
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &line1Rect);                                                           // Ligne horizontale sous nom fusée

    //Affichage boutons au-dessus fenêtre
    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button1Rect);
    SDL_RenderCopy(renderer, imageTextures[2][4], NULL, &button1Rect);

    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button2Rect);


    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button3Rect);

    if (buttonSelected == MOVING_BUTTON){
        SDL_Point centerShipCoord = {shipDestRect.x + shipDestRect.w/2, shipDestRect.y + shipDestRect.h/2};
        plotPath(centerShipCoord, getMouseCoordinates(), 10, 5);
    }
    
}

void changeButtonType(SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &button1Rect)){
        printf("click");
        if (buttonSelected == MOVING_BUTTON){
            buttonSelected = NO_BUTTON;
        }
        else{
            buttonSelected = MOVING_BUTTON;
        }
    }
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
    int planetChosen = whichPlanetIsClicked(planets, planetCount, mouse);
    int shipChosen = whichShipIsClicked(ships, shipCount, mouse);

    if (planetChosen != -1){
        // La nouvelle target est une planète
        ships[idShip].target.type = TARGET_PLANET;
        // Et on indique exactement vers quelle planète le ship doit-il se diriger
        ships[idShip].target.planet = &planets[planetChosen];
    }
    else if (shipChosen != -1){
        // La nouvelle target est un ship [ex une station spatiale, orbitale ou un vaisseau de ravitaillement]
        ships[idShip].target.type = TARGET_SHIP;
        // Et on indique exactement vers quel ship le ship doit-il se diriger
        ships[idShip].target.ship = &ships[shipChosen];
    }
    else { // Si le joueur clique à un endroit random sans rien
        // La nouvelle target est un point random de l'espace
        ships[idShip].target.type = TARGET_POINT;
        // On rendre les coordonnées exactes du point target
        ships[idShip].target.point.x = mouse.x;
        ships[idShip].target.point.y = mouse.y;
    }
}
