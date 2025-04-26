#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include "ship.h"

typedef enum { NO_BUTTON, MOVING_BUTTON, SHIP_WINDOW_BUTTON, ATTACK_BUTTON } BasicShipWindowButton;

void initBasicShipWindow(int i);

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships);

void changeButtonType(SDL_Point mouse);

plotPath(SDL_Point origin, SDL_Point destination, int dashLength, int gapLength);

#endif
