#include "window.h"

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "renderer.h"
#include "tools.h"


// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static SDL_Rect windowRect,
                WindowCrossRect,
                planetDisplayedRect;


void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts) {
    initTextPlanetWindow(textTextures, fonts);
    initRectPlanetWindow(textTextures);
}

void initTextPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts) {
}

void initRectPlanetWindow(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes

    windowRect.x = (1 - 0.8) * SCREEN_WIDTH / 2.;
    windowRect.y = (1 - 0.8) * SCREEN_HEIGHT / 2.;
    windowRect.w = SCREEN_WIDTH * 0.8;
    windowRect.h = SCREEN_HEIGHT * 0.8;

    WindowCrossRect.x = windowRect.x + windowRect.w * 0.97;
    WindowCrossRect.y = windowRect.y + windowRect.h * 0.025;
    WindowCrossRect.w = windowRect.w * 0.015;
    WindowCrossRect.h = windowRect.w * 0.015;
    
    planetDisplayedRect.x = SCREEN_WIDTH / 8.;
    planetDisplayedRect.y = SCREEN_WIDTH / 3.;
    planetDisplayedRect.w = 300;
    planetDisplayedRect.h = 300;
}


void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount) {
    if (getWindowType() != PLANET_WINDOW) {
        return;
    }

    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &windowRect);  // Arriere plan de la fenetre d'informations
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);  // Croix pour fermer la fenetre

    int idPicture = (planets[getWindowId()].planetType == SUN) ? 9 : generateRandNb8(currentSeed, getWindowId()) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &planetDisplayedRect);  // Planete decrite dans cette fenetre
}
