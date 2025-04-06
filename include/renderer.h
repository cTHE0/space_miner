#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "ship.h"

typedef enum { NO_WINDOW, SHIP_WINDOW, PLANET_WINDOW, DEFENDER_WINDOW, ENNEMY_WINDOW } Window;

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void clearScreen(SDL_Renderer *renderer);
void displayGame(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int shipCount, int planetCount);

extern Window windowOpened;

#endif
