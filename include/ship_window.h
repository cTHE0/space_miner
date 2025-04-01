#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"

void addShipWindows();
void deleteShipWindows();
void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Ship ship);
void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures);
void openCloseShipWindowsGestion(int x, int y, int ship_count, Ship *ships);
void freeWindows();

#endif
