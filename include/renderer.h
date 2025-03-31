#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

void initRenderer(SDL_Window **window, SDL_Renderer **renderer);
void clearScreen(SDL_Renderer *renderer);
void presentScreen(SDL_Renderer *renderer);
#endif
