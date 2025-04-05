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
static SDL_Rect planetDisplayedPlanetWindow = {SCREEN_WIDTH / 4., SCREEN_HEIGHT / 3., 300, 300};

void displayPlanetWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount) {
    if (windowOpened == PLANET_WINDOW) {  // Affichage de la fenetre
        SDL_RenderCopy(renderer, imageTextures[21], NULL, &windowRect);
        SDL_RenderCopy(renderer, imageTextures[22], NULL, &WindowCrossRect);
        SDL_RenderCopy(renderer, imageTextures[3 + planets[whichWindowPlanet].idPicture], NULL, &planetDisplayedPlanetWindow);

    }
}
