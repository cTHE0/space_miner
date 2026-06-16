#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"
#include "landing_page.h"

typedef enum {NO_WINDOW, SHIP_WINDOW, PLANET_WINDOW, DEFENDER_WINDOW, ENNEMY_WINDOW, BASIC_SHIP_WINDOW, SIDE_BAR_WINDOW, PAUSE_WINDOW, SETTINGS_WINDOW} WindowType;

typedef struct {
    WindowType type;  // Type de fenetre affiche (d'une fusee, planete, attaquant, etc)
    uint32_t id;      // Stocke l'id de l'objet (fusee, planete, etc) affiche sur la fenetre
    uint32_t lastRefresh;
} Window;

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets);

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, GameState *gameState, SDL_Point mouse, Ship **ships, int *shipCount, Planet *planets, int planetCount);

int whichShipIsClicked(Ship *ships, int shipCount, SDL_Point mouse);
int clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, Planet *planets, SDL_Point mouse);

int whichPlanetIsClicked(Planet *planets, int planetCount, SDL_Point mouse);
int clickOnPlanet(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, Ship *ships, int planetCount, int shipCount, SDL_Point mouse);

int clickOnWindow(SDL_Point mouse);

void setSelectionMode(int newSelectionMode);

void setWindowId(int newId);
int getWindowId(void);

WindowType getWindowType(void);
void setWindowType(WindowType newType);

uint32_t getWindowLastRefresh(void);
void setWindowLastRefresh(uint32_t newRefresh);

void displaySelectionCircle(void);
void objetInSelectionCircle(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ships, int shipCount, Planet *planets, int planetCount);

void updateGame(SDL_Texture **textTextures, TTF_Font **fonts, Ship **ships, int *shipCount, Planet *planets, int planetCount, Mix_Chunk **sounds);
void updateWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, Planet *planets, int shipCount);

#endif
