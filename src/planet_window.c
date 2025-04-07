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


int whichWindowPlanet = -1;
static SDL_Rect planetDisplayedRect = {SCREEN_WIDTH / 4., SCREEN_HEIGHT / 3., 300, 300};

void displayPlanetWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount) {
    if (windowOpened == PLANET_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &windowRect);  // Arriere plan de la fenetre d'informations
        SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);  // Croix pour fermer la fenetre
        SDL_RenderCopy(renderer, imageTextures[5][planets[whichWindowPlanet].idPicture], NULL, &planetDisplayedRect);  // Planete decrite dans cette fenetre

    }
}
