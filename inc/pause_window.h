#ifndef PAUSE_WINDOW_H
#define PAUSE_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "landing_page.h"
#include "planet.h"
#include "ship.h"


void initPauseWindow(SDL_Texture **textTextures, TTF_Font **fonts);
void initPauseWindowRects(SDL_Texture **textTextures);

void displayPauseWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
	void pauseWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
	void pauseWindowButtons(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void pauseWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, GameState *gameState, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount);

void saveGame(Ship *ships, int shipCount, Planet *planets, int planetCount);

#endif
