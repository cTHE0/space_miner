#include "planet.h"
#include "camera.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void generatePlanets(Planet **planets, int count) {
    *planets = malloc(count * sizeof(Planet));

    if (*planets == NULL) {
        printf("Erreur d'allocation mémoire pour les planetes !\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *planets après un échec
    }

    for (int i = 0; i < count; i++) {
        float radius = 10 + rand() % 50;  // Taille entre 20 et 30
        (*planets)[i].radius = radius;
        // + radius pour que ((*planets)[i].x, (*planets)[i].y) soit le centre de la planète
        (*planets)[i].x = (rand() % MAP_WIDTH) + radius;
        (*planets)[i].y = (rand() % MAP_HEIGHT) + radius;
    }
    
}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture *texturePlanet, int count) {
    for (int i = 0; i < count; i++) {
        float screenX = (planets[i].x - camera.rect.x) * camera.scale + SCREEN_WIDTH / 2;  //(screenX, screenY) = coordonnées écran
        float screenY = (planets[i].y - camera.rect.y) * camera.scale + SCREEN_HEIGHT / 2;  //(planets[i].x, planets[i].y) = coordonnées monde
        float screenRadius = planets[i].radius * camera.scale;  
        SDL_Rect srcRect = {0, 0, 32, 32};
        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        // On n'affiche pas les planètes situés en dehors du cadre
        if (screenX > 0 && screenX + 2*screenRadius < SCREEN_WIDTH && 
            screenY > 0 && screenY + 2*screenRadius < SCREEN_HEIGHT) {        
            SDL_RenderCopy(renderer, texturePlanet, &srcRect, &destRect);
        }   
    }
}
