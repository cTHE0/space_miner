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
#include "camera.h"

SDL_Rect windowRect = {(1 - 0.8) * SCREEN_WIDTH / 2., 
                       (1 - 0.8) * SCREEN_HEIGHT / 2., 
                       SCREEN_WIDTH * 0.8,
                       SCREEN_HEIGHT * 0.8};
SDL_Rect WindowCrossRect = {(1 + 0.8) / 2. * SCREEN_WIDTH - (SCREEN_WIDTH * 0.8) * 0.03, 
                            (1 - 0.8) * SCREEN_HEIGHT / 2 + 20,
                            (SCREEN_WIDTH * 0.8) * 0.015,
                            (SCREEN_WIDTH * 0.8) * 0.015};

void openWindowGestion(int x, int y, int shipCount, Planet *planets, int planetCount) {
    SDL_Point mouse = {x, y};

    if (windowOpened == NO_WINDOW) {
        clickOnShip(shipCount, mouse);
        clickOnPlanet(planets, planetCount, mouse);
    } else if (SDL_PointInRect(&mouse, &WindowCrossRect)){
        windowOpened = NO_WINDOW;
    }
}

void clickOnShip(int shipCount, SDL_Point mouse) {
    for (int i = 0; i < shipCount; i++) {
        if (SDL_PointInRect(&mouse, &ships[i].destRect)) {
            whichWindowShip = i;
            windowOpened = SHIP_WINDOW;
            initShipWindow();
            return;
        }
    }
}

void clickOnPlanet(Planet *planets, int planetCount, SDL_Point mouse) {
    for (int i = 0; i < planetCount; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point au milieu de la planete
        // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenX = (planets[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;
        float screenY = (planets[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * camera.scale;  
        if (fabs(screenX - mouse.x) < screenRadius && fabs(screenY - mouse.y) < screenRadius) {
            whichWindowPlanet = i;
            windowOpened = PLANET_WINDOW;
            return;
        }
    }
}

void displayWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int planetCount) {
    displayShipWindow(renderer, imageTextures, textTextures, ships);
    displayPlanetWindow(renderer, imageTextures, textTextures, planets, planetCount);
}