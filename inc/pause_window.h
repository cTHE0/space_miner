#ifndef PAUSE_WINDOW_H
#define PAUSE_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "landing_page.h"
#include "planet.h"
#include "ship.h"


//void initPauseWindow(SDL_Texture **textTextures, TTF_Font **fonts);
void initPauseWindowRects(SDL_Texture **textTextures);

void displayPauseWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
	void pauseWindowFondations();
	void pauseWindowAesthetic(SDL_Texture ***imageTextures);
	void pauseWindowButtons(SDL_Texture **textTextures);
void pauseWindowGestion(Mix_Chunk **sounds, GameState *gameState, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount);

void initSettingsWindowRects(void);
void displaySettingsWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
int settingsWindowGestion(Mix_Chunk **sounds, SDL_Point mouse);  // renvoie 1 si fermeture demandee

void displayGameOverWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void gameOverWindowGestion(GameState *gameState, Mix_Chunk **sounds, SDL_Point mouse);

void displayVictoryWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void victoryWindowGestion(GameState *gameState, Mix_Chunk **sounds, SDL_Point mouse);

void saveGame(Ship *ships, int shipCount, Planet *planets, int planetCount);

#endif
