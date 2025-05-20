#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ship.h"

typedef enum { NO_BUTTON, SHIP_WINDOW_BUTTON, BASE_BUTTON, TARGET_BUTTON, ATTACK_BUTTON } BasicShipWindowButton;

void displayBasicShipWindow(SDL_Texture ***imageTextures, Ship *ships);
void basicShipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse);
void choosingNewBaseOrTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse);
int clickOnBasicShipWindow(SDL_Point mouse);

#endif
