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
        float radius = 10 + rand() % 50;  // Taille entre 10 et 60

        (*planets)[i].radius = radius;
        (*planets)[i].x = (rand() % (int)(MAP_WIDTH - 2 * radius)) + (int)radius;
        (*planets)[i].y = (rand() % (int)(MAP_HEIGHT - 2 * radius)) + (int)radius;
    }
    
}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture *texturePlanet, int count) {
    for (int i = 0; i < count; i++) {
        //(screenX, screenY) = coordonnees  sur l'ecran physique de la planete (point en haut à gauche du rect)
        float screenX = (planets[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;  //(planets[i].x, planets[i].y) = coordonnees sur la map
        float screenY = (planets[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * camera.scale;  

        SDL_Rect srcRect = {0, 0, 32, 32};
        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        if (screenX > -2 * screenRadius && screenX < MAP_WIDTH + 2 * screenRadius && 
            screenY > -2 * screenRadius && screenY < MAP_HEIGHT + 2 * screenRadius) {   // On n'affiche pas les planètes situés en dehors du cadre     
            SDL_RenderCopy(renderer, texturePlanet, &srcRect, &destRect);
        }   
    }
}
