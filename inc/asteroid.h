#ifndef ASTEROID_H
#define ASTEROID_H

#include "planet.h"
#include <SDL2/SDL.h>


void initAsteroids(Planet *planets, int planetCount);
void updateAsteroids(Planet *planets, int planetCount);
void displayAsteroid(SDL_Texture ***imageTextures);
void destroyAsteroids(void);

#endif