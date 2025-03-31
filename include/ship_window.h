#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"

void addShipWindows();
void deleteShipWindows();
void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Ship ship);
void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures);
void freeWindows();

#endif
