#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"


typedef struct
{
    Ship ship;
    SDL_Rect destRect;
    SDL_Rect crossRect;
} ShipWindow;


void addShipWindow(Ship ship);
void deleteShipWindows();
void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, ShipWindow ship_window);
void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures);
void openCloseShipWindowsGestion(int x, int y, int ship_count, Ship *ships);
void freeWindows();
int whichWindow(int mouseX, int mouseY);
void moveWindow(int dx, int dy, int i);


#endif
