#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

typedef enum {SUN, PLANET} planetTypeEnum; 

typedef struct {
    int id;                     // Identifiant de la planet (permet la generation de nom aleatoire, et de position aleatoire bientot)
    planetTypeEnum planetType;  // Type de planete : soleil, planete gazeuse, planete rocheuse, ...
    float x, y;                 // Position du centre de la planete sur la map
    float radius;               // Rayon de la planete
    float maxOre, currentOre;   // Quantité maximale et actuelle de minerai (on demarre avec un seul type de minerai, generalisation facile)
    float regenerationTime;     // Duree pour que la planete regenere ses ressources lorsque que currentOre = 0
} Planet;

void generatePlanets(Planet **planets, int planetCount);
void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount);

#endif
