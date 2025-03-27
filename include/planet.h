#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;  // Position du centre de la planete sur la map
    float radius; // Rayon
} Planet;

void generatePlanets(Planet **planets, int count);
void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture *texturePlanet, int count);

#endif
