#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>
#include "config.h"
#include "build.h"

typedef enum { SUN, PLANET } planetTypeEnum;

typedef struct {
    int id;                             // Identifiant de la planète
    planetTypeEnum planetType;          // Type de planète
    float x, y;                         // Position du centre de la planète sur la map
    float radius;
    float maxOre[ORE_TYPE_COUNT];       // Quantité maximale et actuelle de minerais
    float currentOre[ORE_TYPE_COUNT];
    Build builds[BUILD_TYPE_COUNT];     // Bâtiments construits sur la planète
    int abundance[ORE_TYPE_COUNT];      // Pourcentage de difficulté de minage

    /*---Gestion rotation autour soleil---*/
    double orbit_radius;
    double orbit_angle_deg;
    double orbit_speed_deg;             // vitesse propre à chaque planète
    int orbit_center_x;
    int orbit_center_y;
} Planet;

void updatePlanet(Planet *planets);
void generatePlanets(Planet **planets, int planetCount);
void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount);
void destroyPlanets(Planet *planets, int planetCount);
void displayBuildAroundPlanet(Planet planet, int nb_build, SDL_Texture **build_textures_list, SDL_Point texture_dimensions);

#endif
