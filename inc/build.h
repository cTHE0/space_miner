#ifndef BUILD_H
#define BUILD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ore.h"

typedef enum { ORE_STORE, ORE_MINE, DEFENCE_TOWER, FACTORY } BuildType;



typedef struct Planet Planet;

typedef struct Build {
    BuildType type;
    int level;
    union {
        Compartment tank;    // Si c'est un réservoir
        int damages;         // Si tour de défense
        int productionSpeed; // Si c'est une factory ou une mine
    };
} Build ;

void initBuildsPlanet(Planet *planet);
void displayBuildsAroundPlanet(Planet *planet, int nb_build, SDL_Texture **build_textures_list, SDL_Point texture_dimensions);

#endif
