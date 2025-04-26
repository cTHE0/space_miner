#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"
#include "config.h"

SDL_Rect rangeCircle;
static SDL_Rect bgRect = {SCREEN_WIDTH/12, 3*SCREEN_HEIGHT/4, 5*SCREEN_WIDTH/6, SCREEN_HEIGHT/4};
static SDL_Rect shipPicture = {SCREEN_WIDTH/6, 3*SCREEN_HEIGHT/4, SCREEN_HEIGHT/4, SCREEN_HEIGHT/4};

int idShip; //identifiant du ship dont la basic window est à afficher

void initBasicShipWindow(int i){
    idShip = i;
}

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships){
    if (getWindowType() != BASIC_SHIP_WINDOW) {  // La fenetre d'informations [basiques] d'une fusee est-elle ouverte ?
        return;
    }
    SDL_Rect shipDestRect = getShipOnScreen(ships[idShip]);

    rangeCircle = (SDL_Rect){shipDestRect.x - ships[idShip].range, shipDestRect.y - ships[idShip].range, 2*ships[idShip].range, 2*ships[idShip].range};
    SDL_RenderCopy(renderer, imageTextures[5][4], NULL, &rangeCircle);
    // + 1% des parts à la personne qui lira ce message
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);
    SDL_RenderCopy(renderer, imageTextures[8][0], NULL, &shipPicture);
}