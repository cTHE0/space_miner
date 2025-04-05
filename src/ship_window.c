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

void displayShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount) {
    if (windowOpened == SHIP_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[21], NULL, &windowRect);
        SDL_RenderCopy(renderer, imageTextures[22], NULL, &WindowCrossRect);
    }
}
