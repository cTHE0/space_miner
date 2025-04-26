#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"

SDL_Rect rangeCircle;

int idShip; //identifiant du ship dont la basic window est à afficher

void initBasicShipWindow(int i){
    idShip = i;
}

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships){
    if (getWindowType() != BASIC_SHIP_WINDOW) {  // La fenetre d'informations [basiques] d'une fusee est-elle ouverte ?
        return;
    }
    printf("%d", idShip);
    SDL_Rect shipDestRect = getShipOnScreen(ships[idShip]);

    rangeCircle = (SDL_Rect){shipDestRect.x - ships[idShip].range, shipDestRect.y - ships[idShip].range, 2*ships[idShip].range, 2*ships[idShip].range};
    SDL_RenderCopy(renderer, imageTextures[4][4], NULL, &rangeCircle);
    // + 1% des parts à la personne qui lira ce message
}