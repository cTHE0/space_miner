#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;   // Position
    float radius; // Rayon
} Planet;

void generatePlanets(Planet **planets, int count);
void renderPlanets(SDL_Renderer *renderer, Planet *planets, int count);

#endif
