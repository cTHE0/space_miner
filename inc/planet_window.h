#ifndef PLANET_WINDOW_H
#define PLANET_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"


void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets);
void initRectPlanetWindow(SDL_Texture **textTextures);
void initTextPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets);

void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowFoundations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void planetWindowGeneralInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowContainerInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowMineralAbundance(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowManageBuilds(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet);
void planetWindowOverviewBuild(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet);
void planetWindowNearestShips(SDL_Texture ***imageTextures, SDL_Texture **textTextures);

void planetWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, SDL_Point mouse);

#endif
