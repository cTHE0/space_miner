#ifndef BUILD_H
#define BUILD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ore.h"
#include "mine.h"

typedef enum { ORE_STORE, ORE_MINE, DEFENCE_TOWER, FACTORY, NOTHING } BuildType;

typedef struct Planet Planet;
typedef struct Ship Ship;

typedef struct Build {
    BuildType type;
    int level;
    union {
        Compartment tank;    // Si c'est un réservoir
        int damages;         // Si tour de défense
        Mine mine;
    };
} Build;


void initBuildsPlanet(Planet *planet);
void displayBuildsAroundPlanet(Planet *planet, int nb_build, SDL_Texture **build_textures_list, SDL_Point texture_dimensions);
void updateBuilds(Planet *planets, Ship *ships, int shipCount, int planetCount);
void updateBuildMine(Build *builds, Mine *mine, int abundance);

#endif
