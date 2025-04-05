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
static SDL_Rect shipDisplayedRect = {SCREEN_WIDTH / 4., SCREEN_HEIGHT / 3., 300, 300};
static SDL_Rect srcRectShip = {0, 0, 64, 64};

void displayShipWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount) {
    if (windowOpened == SHIP_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[6][1], NULL, &windowRect);
        SDL_RenderCopy(renderer, imageTextures[0][ships[whichWindowShip].idPicture], &srcRectShip, &shipDisplayedRect);
        SDL_RenderCopy(renderer, imageTextures[4][1], NULL, &WindowCrossRect);
    }
}
