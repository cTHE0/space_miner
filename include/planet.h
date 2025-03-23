#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

typedef enum { LOD_HIGH, LOD_MEDIUM, LOD_LOW } LODLevel;

typedef struct {
    float x, y;   // Position
    float radius; // Rayon
} Planet;

void generatePlanets(Planet *planets, int count);
LODLevel getLOD(float screenRadius);
void renderPlanets(SDL_Renderer *renderer, Planet *planets, int count);

#endif
