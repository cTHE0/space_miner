#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>
#include "config.h"
#include "build.h"
#include "ship.h"
#include "build.h"

#include "ore.h"
#include "mine.h"
typedef enum { SUN, PLANET } planetTypeEnum;

typedef struct Ship Ship;

typedef struct Planet {
    int id;                             // Identifiant de la planète
    planetTypeEnum planetType;          // Type de planète
    float x, y;                         // Position du centre de la planète sur la map
    float radius;
    Build builds[BUILD_TYPE_COUNT];     // Bâtiments construits sur la planète
    int abundance[ORE_TYPE_COUNT];      // Pourcentage de difficulté de minage

    /*---Gestion rotation autour soleil---*/
    double orbitRadius;
    double orbitAngleDeg;
    double orbitSpeedDeg;             // vitesse propre à chaque planète
    int orbitCenterX;
    int orbitCenterY;
} Planet;

int getNbSolarSystems();
void updatePlanets(Planet *planets, Ship *ships, int shipCount, int planetCount);
void generatePlanets(Planet **planets, int planetCount);
void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount);
void destroyPlanets(Planet *planets);

#endif
