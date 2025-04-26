#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"

void initSDL(SDL_Window **window);
void clearScreen(void);
void displayGame(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount, Planet *planets, int planetCount, int pointCount, int nbPointsPerLigne);
void quitSDL(SDL_Window *window);

extern SDL_Renderer *renderer;

#endif
