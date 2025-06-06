#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"

typedef enum {NO_WINDOW, SHIP_WINDOW, PLANET_WINDOW, DEFENDER_WINDOW, ENNEMY_WINDOW, BASIC_SHIP_WINDOW} WindowType;

typedef struct {
    WindowType type;  // Type de fenetre affiche (d'une fusee, planete, attaquant, etc)
    uint32_t id;      // Stocke l'id de l'objet (fusee, planete, etc) affiche sur la fenetre
} Window;

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int planetCount);

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount);

int whichShipIsClicked(Ship *ships, int shipCount, SDL_Point mouse);
int clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, SDL_Point mouse);

int whichPlanetIsClicked(Planet *planets, int planetCount, SDL_Point mouse);
int clickOnPlanet(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, int planetCount, SDL_Point mouse);

int clickOnWindow(SDL_Point mouse);

WindowType getWindowType(void);
int getWindowId(void);

void setWindowType(WindowType newType);
void setWindowId(int newId);

#endif
