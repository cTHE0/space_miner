#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "landing_page.h"
#include "planet.h"
#include "ship.h"

void handleEvents(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, GameState *state, Ship *ships, int shipcount, Planet *planets, int planetCount);
SDL_Point getMouseCoordinates(void);

#endif
