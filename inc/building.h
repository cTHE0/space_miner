#ifndef BUILDING_H
#define BUILDING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ore.h"

typedef enum {ORE_STORE, DEFENCE_TOWER, LABORATORY, OBSERVATION_TOWER} BuildingType;

typedef struct {
    BuildingType type;
    union {
        Compartment tank;    // Si c'est un reservoir
        // Si tour defense
        // Si laboratoire
        // Si tour d'observation
    };
} Building;



#endif
