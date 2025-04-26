#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include "ship.h"

void initBasicShipWindow(int i);

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships);


#endif
