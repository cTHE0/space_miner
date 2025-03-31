#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "ship.h"

void initRenderer(SDL_Window **window, SDL_Renderer **renderer);
void clearScreen(SDL_Renderer *renderer);
void displayGame(SDL_Renderer *renderer, SDL_Texture **imageTextures, Ship *ships, Planet *planets, int ship_count, int planet_count);

#endif
