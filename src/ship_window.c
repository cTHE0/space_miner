#include "window.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"


int whichWindowShip = -1;
static SDL_Rect targetDisplayedRect = {SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.35, SCREEN_WIDTH * 0.2, SCREEN_WIDTH * 0.2};
static SDL_Rect baseDisplayedRect = {SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.35, SCREEN_WIDTH * 0.2, SCREEN_WIDTH * 0.2};
static SDL_Rect srcRectShip = {0, 0, 64, 64};
static SDL_Rect destRectShip = {SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.427, SCREEN_WIDTH * 0.1, SCREEN_WIDTH * 0.1};

void displayShipWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount) {
    if (windowOpened == SHIP_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &windowRect);
        SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].target->idPicture], NULL, &targetDisplayedRect);
        SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].base->idPicture], NULL, &baseDisplayedRect);
        SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);

        if (ships[whichWindowShip].state == MOVING_TO_TARGET || ships[whichWindowShip].state == WAITING_ON_BASE) {  
            float f = distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base)
                      / distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base);  // Fraction du chemin parcourue
            // Tracer la fleche
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (int i = 0; i < SCREEN_WIDTH * 0.30; i += 23) { 
                SDL_Rect narrowRect = {SCREEN_WIDTH * 0.35 + i, SCREEN_HEIGHT / 2, 10, 40};
                SDL_RenderFillRect(renderer, &narrowRect);

                if (f < i / (SCREEN_WIDTH * 0.3)) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
            }

            // Tracer la fusee
            destRectShip.x = f * SCREEN_WIDTH * 0.3 + SCREEN_WIDTH * 0.3;
            SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 90, NULL, SDL_FLIP_NONE);

        } else if (ships[whichWindowShip].state == MOVING_TO_BASE || ships[whichWindowShip].state == WAITING_ON_TARGET) { 
            float f = distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base)
                      / distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base);  // Fraction du chemin parcourue             
            // Tracer la fleche
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (int i = 0; i < SCREEN_WIDTH * 0.30; i += 26) { 
                SDL_Rect narrowRect = { SCREEN_WIDTH * 0.35 + i, SCREEN_HEIGHT / 2, 10, 40};
                SDL_RenderFillRect(renderer, &narrowRect);
                if (f < i / (SCREEN_WIDTH * 0.3)) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
            }

            // Tracer la fusee
            destRectShip.x = f * SCREEN_WIDTH * 0.3 + SCREEN_WIDTH * 0.3;
            SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 270, NULL, SDL_FLIP_NONE);
        }
    }
}