#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include "ship.h"

typedef enum { MOVING_BUTTON, SHIP_WINDOW_BUTTON, ATTACK_BUTTON } BasicShipWindowButton;

void initBasicShipWindow(int i);

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships);

#endif
