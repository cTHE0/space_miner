#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"


void initShipWindowRects(SDL_Texture **textTextures);
void initShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship);

void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets);
void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets);
void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets);
void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
void ShipWindowShipModel(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);

void shipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ship, Planet *planets, SDL_Point mouse);

#endif
