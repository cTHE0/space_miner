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
SDL_Rect targetDisplayedRect;
SDL_Rect baseDisplayedRect;
static SDL_Rect srcRectShip = {0, 0, 64, 64};
SDL_Rect destRectShip;

void displayShipWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount) {
    baseDisplayedRect = (SDL_Rect){windowRect.x + 2*windowRect.w/32, windowRect.y + 2.5*windowRect.h/16, windowRect.w * 0.12, windowRect.w  * 0.12};
    targetDisplayedRect = (SDL_Rect){windowRect.x + 7*windowRect.w/16, windowRect.y + 2.5*windowRect.h/16, windowRect.w * 0.12, windowRect.w  * 0.12};
    destRectShip = (SDL_Rect){0, baseDisplayedRect.y + baseDisplayedRect.w/2 - windowRect.w*0.046, windowRect.w*0.1, windowRect.w*0.1};
    if (windowOpened == SHIP_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &windowRect);
        SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].target->idPicture], NULL, &targetDisplayedRect);
        SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].base->idPicture], NULL, &baseDisplayedRect);
        SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        //Texte
        SDL_RenderCopy(renderer, textTextures[4], NULL, &(SDL_Rect){1.1*windowRect.x, 1.1*windowRect.y, windowRect.w/3, windowRect.h/8});

        //Barres (oui c chiant bordel de merde)
        SDL_RenderFillRect(renderer, &((SDL_Rect){windowRect.x, windowRect.y + windowRect.h/16, windowRect.w, 3})); //1ère barre horizontale
        SDL_RenderFillRect(renderer, &((SDL_Rect){windowRect.x + 5*windowRect.w/8, windowRect.y + windowRect.h/16, 3, windowRect.h})); //barre verticale milieu
        SDL_RenderFillRect(renderer, &((SDL_Rect){windowRect.x, 5*windowRect.h/8, 5*windowRect.w/8, 3})); //2ème barre horizontale

        //Tracer en pointilliés entre planètes
        SDL_Rect narrowRect = { baseDisplayedRect.x + baseDisplayedRect.w, baseDisplayedRect.y + baseDisplayedRect.h/2, 5, 15};
        int pas = 10; //Pas des pointilliés
        int dp = targetDisplayedRect.x-(baseDisplayedRect.x + baseDisplayedRect.w); // distance en pixel entre 2 planètes sur fenêtre
        float f;

        if (ships[whichWindowShip].state == MOVING_TO_TARGET || ships[whichWindowShip].state == WAITING_ON_BASE) {  
            f = (distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base) - (*ships[whichWindowShip].base).radius - (*ships[whichWindowShip].target).radius)
                      / distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base);  // Fraction du chemin parcourue
            // Tracer la fleche
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (int i = 0; i < dp; i += pas) { 
                narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w + i;
                SDL_RenderFillRect(renderer, &narrowRect);

                if (f*dp< i) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
            }

            // Tracer la fusee
            destRectShip.x = (int)(f * dp) + baseDisplayedRect.x + baseDisplayedRect.w - destRectShip.w*0.25;
            SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 90, NULL, SDL_FLIP_NONE);

        } else if (ships[whichWindowShip].state == MOVING_TO_BASE || ships[whichWindowShip].state == WAITING_ON_TARGET) { 
            f = (distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base) - (*ships[whichWindowShip].base).radius - (*ships[whichWindowShip].target).radius)
                      / distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base);  // Fraction du chemin parcourue             
            // Tracer la fleche
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (int i = 0; i < dp; i += pas) { 
                narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w + i;
                SDL_RenderFillRect(renderer, &narrowRect);
                if (f*dp< i) {

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
            }
            // Tracer la fusee
            destRectShip.x = (int)(f * dp) + baseDisplayedRect.x + baseDisplayedRect.w - destRectShip.w*0.25;
            SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 270, NULL, SDL_FLIP_NONE);
        }
    }
}