#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

typedef struct {
    int idPicture;              // Donnee du skin de la planete (depend du numero des noms d'image dans assets/img/planets)
    float x, y;                 // Position du centre de la planete sur la map
    float radius;               // Rayon de la planete
    float maxOre, currentOre;   // Quantité maximale et actuelle de minerai (on demarre avec un seul type de minerai, generalisation facile)
    float regenerationTime;     // Duree pour que la planete regenere ses ressources lorsque que currentOre = 0
} Planet;

void generatePlanets(Planet **planets, int count);
void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture **texturePlanet, int count);

#endif
