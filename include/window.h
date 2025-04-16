#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"

void displayWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int shipCount, int planetCount);
    void displayShipWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount);
    void initRectShipWindow(SDL_Texture **textTextures);

    void displayPlanetWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount);

void openWindowGestion(int x, int y, Ship *ships, int shipCount, Planet *planets, int planetCount);
    void clickOnShip(Ship *ships, int shipCount, SDL_Point mouse);
    void clickOnPlanet(Planet *planets, int planetCount, SDL_Point mouse);

extern int whichWindowShip;
extern int whichWindowPlanet;
extern SDL_Rect windowRect;
extern SDL_Rect WindowCrossRect;

#endif
