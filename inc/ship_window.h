#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"


void initShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship);
void initRectShipWindow(SDL_Texture **textTextures);
void initTextShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship);

void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowShipModel(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);

void refreshRectTankchoosen(SDL_Texture **textTextures);

void shipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship, Planet *planets, SDL_Point mouse);

#endif
