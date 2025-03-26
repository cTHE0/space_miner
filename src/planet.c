#include "planet.h"
#include "camera.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void generatePlanets(Planet **planets, int count) {
    *planets = malloc(count * sizeof(Planet));

    if (*planets == NULL) {
        printf("Erreur d'allocation mémoire pour les planetes !\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *planets après un échec
    }

    for (int i = 0; i < count; i++) {
        (*planets)[i].x = rand() % SCREEN_WIDTH;
        (*planets)[i].y = rand() % SCREEN_HEIGHT;
        (*planets)[i].radius = 20 + rand() % 10;  // Taille entre 20 et 30
    }
    
}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, int count) {
    for (int i = 0; i < count; i++) {
        float screenX = (planets[i].x - camera.rect.x) * camera.scale + SCREEN_WIDTH / 2;  //(screenX, screenY) = coordonnées écran
        float screenY = (planets[i].y - camera.rect.y) * camera.scale + SCREEN_HEIGHT / 2;  //(planets[i].x, planets[i].y) = coordonnées monde
        float screenRadius = planets[i].radius * camera.scale;  

        if (screenX + screenRadius > 0 && screenX - screenRadius < SCREEN_WIDTH && 
            screenY + screenRadius > 0 && screenY - screenRadius < SCREEN_HEIGHT) {
            filledCircleRGBA(renderer, (int)screenX, (int)screenY, (int)screenRadius, 255, 255, 255, 255);
        }   
    }
}
