#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>
#include "config.h"
#include "building.h"

typedef enum {SUN, PLANET} planetTypeEnum; 

typedef struct {
    int id;                             // Identifiant de la planet (permet la generation aleatoire
    planetTypeEnum planetType;          // Type de planete : soleil, planete gazeuse, planete rocheuse, ...
    float x, y;                         // Position du centre de la planete sur la map
    float radius;
    float     maxOre[ORE_TYPE_COUNT];   // Quantite maximale et actuelle de minerais
    float currentOre[ORE_TYPE_COUNT];
    Building *buildings;                // Batiments construits sur la planete
    int buildingCounter;
    float regenerationTime;             // Duree pour que la planete regenere ses ressources lorsque que currentOre = 0
} Planet;

void generatePlanets(Planet **planets, int planetCount);
void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount);
void destroyPlanets(Planet *planets, int planetCount);

#endif
