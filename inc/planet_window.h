#ifndef PLANET_WINDOW_H
#define PLANET_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"


void initPlanetWindowRects(SDL_Texture **textTextures);
void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, Ship *ships, int shipCount);

void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, Ship *ships);
void planetWindowFoundations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void planetWindowGeneralInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowContainerInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet);
void planetWindowMineralAbundance(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets);
void planetWindowManageBuilds(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet);
void planetWindowOverviewBuild(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet);
void planetWindowNearestShips(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);

void planetWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship **ships, int *shipCount, Planet *planets, int planetCount, SDL_Point mouse);

void computeCloseShipsFromPlanet(Planet *planet, Ship *ships, int shipCount);

void setCurrentBuildIndex(int index);

#endif
