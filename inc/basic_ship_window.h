#ifndef BASIC_SHIP_WINDOW_H
#define BASIC_SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "ship.h"


void initBasicShipWindowRects(SDL_Texture **textTextures);
void initBasicShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship);
	void updateNarrowBasicShipWindow(Ship *ship);

typedef enum { NO_BUTTON, SHIP_WINDOW_BUTTON, BASE_BUTTON, TARGET_BUTTON, ATTACK_BUTTON } BasicShipWindowButton;

void displayBasicShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
	void basicShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture ** textTextures);
	void basicShipWindowNarrowBaseTarget(SDL_Texture ***imageTextures);
	void basicShipWindowInfos(SDL_Texture ***imageTextures, SDL_Texture ** textTextures, Ship *ships);
void basicShipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse);
void choosingNewBaseOrTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse);
int clickOnBasicShipWindow(SDL_Point mouse);

void setButtonSelected(BasicShipWindowButton newButton);
BasicShipWindowButton getButtonSelected(void);


#endif
