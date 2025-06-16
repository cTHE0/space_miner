#ifndef ASTEROID_H
#define ASTEROID_H

#include "planet.h"
#include <SDL2/SDL.h>

void initAsteroids(Planet *planets, int planet_count);
void updateAsteroids(Planet *planets, int planetCount);
void displayAsteroid(SDL_Texture ***imageTextures);
void freeAsteroid();

#endif