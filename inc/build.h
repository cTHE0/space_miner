#ifndef BUILD_H
#define BUILD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ore.h"

typedef enum { ORE_STORE, ORE_MINE, DEFENCE_TOWER, FACTORY } BuildType;

typedef union {
    Compartment tank;    // Si c'est un réservoir
    int damages;         // Si tour de défense
    int production_speed; // si c'est une factory ou une mine
} BuildData;

typedef struct {
    BuildType type;
    int level;
    BuildData data;
} Build;

#endif
