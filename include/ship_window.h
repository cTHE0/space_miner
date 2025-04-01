#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"

typedef enum { VISIBLE, NOT_VISIBLE } ShipWindowVisibility;


typedef struct
{
    Ship ship;
    SDL_Rect destRect;
    SDL_Rect crossRect;
    ShipWindowVisibility visibility;
} ShipWindow;


void addShipWindows();
void deleteShipWindows();
void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, ShipWindow ship_window);
void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures);
void openCloseShipWindowsGestion(int x, int y, int ship_count, Ship *ships);
void freeWindows();

#endif
