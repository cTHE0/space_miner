#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"
#include "config.h"
#include "camera.h"

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

int idShip; //identifiant du ship dont la basic window est à afficher
float cameraScale;

void initBasicShipWindow(int i){
    idShip = i;
}

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
}