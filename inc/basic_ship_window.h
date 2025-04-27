#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include "ship.h"

typedef enum { NO_BUTTON, MOVING_BUTTON, SHIP_WINDOW_BUTTON, ATTACK_BUTTON } BasicShipWindowButton;

void initBasicShipWindow(int i);

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships);

int changeButtonType(SDL_Point mouse);

void plotPath(SDL_Point origin, SDL_Point destination, int dashLength, int gapLength);

void choosingNewTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse);

#endif
