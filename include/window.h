#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"

typedef enum { NO_WINDOW, SHIP_WINDOW, PLANET_WINDOW, DEFENDER_WINDOW, ENNEMY_WINDOW } Window;

void initTextShipWindow(void);
void initShipWindow(void);

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, int planetCount);
    void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void initRectShipWindow(void);
        void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures);

    void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, int planetCount);

void openWindowGestion(int x, int y, int shipCount, int planetCount);
    void clickOnShip(int shipCount, SDL_Point mouse);
    void clickOnPlanet(int planetCount, SDL_Point mouse);

extern Window windowOpened;
extern int whichWindowShip;
extern int whichWindowPlanet;

extern SDL_Rect windowRect;
extern SDL_Rect WindowCrossRect;

#endif
