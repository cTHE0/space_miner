#ifndef BUIL_H
#define BUILD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ore.h"

typedef enum {ORE_STORE, DEFENCE_TOWER} BuildType;

typedef struct {
    BuildType type;
    int level; //level=0 si build pas encore construit
    union {
        Compartment tank;    // Si c'est un reservoir
        // Si tour defense
        // Si laboratoire
        // Si tour d'observation
    };
} Build;



#endif
