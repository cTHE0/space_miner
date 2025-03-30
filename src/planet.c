#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "planet.h"
#include "camera.h"
#include "config.h"

void generatePlanets(Planet **planets, int count) {
    *planets = malloc(count * sizeof(Planet));

    for (int i = 0; i < count; i++) {
        printf("%d\n",i);
        (*planets)[i].maxOre = 1 + rand () % 1000;  // Les planetes ne sont pas toutes rentables
        (*planets)[i].currentOre = rand () % (int)(*planets)[i].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
        (*planets)[i].regenerationTime = rand () % 10000;  // En millisecondes

        // Recherche d'une nouvelle planete eloignee de toutes les autres
        short planetIsAlone = 0;  // 0: la nouvelle planete est proche d'une autre, 1: la nouvelle planete est eloignee
        int r, x, y;
        while (planetIsAlone == 0) {
            r = 10 + rand() % 50;
            x = rand() % (MAP_SIZE - 2 * r) + r;
            y = rand() % (MAP_SIZE - 2 * r) + r;
            planetIsAlone = 1;  // La nouvelle planete est eloignee A PART SI l'on en detecte une autre a cote

            for (int j = 0; j < i; j++) {  // Verifie qu'il n'y ait pas de planetes trop proches de la planete i
                if (planetIsAlone == 1 && 
                    (abs(x - (int)(*planets)[j].x) < r + (*planets)[j].radius ||
                     abs(y - (int)(*planets)[j].y) < r + (*planets)[j].radius)) {
                    planetIsAlone = 0;
                }
            }
        }
        (*planets)[i].radius = r;
        (*planets)[i].x = x;
        (*planets)[i].y = y;
    }
}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture *texturePlanet, int count) {
    for (int i = 0; i < count; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point en haut à gauche du rect de la planete
        float screenX = (planets[i].x - camera.rect.x - planets[i].radius - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenY = (planets[i].y - camera.rect.y - planets[i].radius - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * camera.scale;  

        SDL_Rect srcRect = {0, 0, 32, 32};
        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        if (screenX >= -2 * screenRadius && screenX <= SCREEN_WIDTH && 
            screenY >= -2 * screenRadius && screenY <= SCREEN_HEIGHT + screenRadius) {   // On n'affiche pas les planètes situés en dehors de l'ecran  
            // Affichage planete 
            SDL_RenderCopy(renderer, texturePlanet, &srcRect, &destRect);

            // Affichage barre* de minerais (1)      (*une seule barre, mais representant la valeur totale de minerais !?)
            destRect.h = 5 * camera.scale;
            destRect.y -= destRect.h + 2 / camera.scale;

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &destRect);

            // Affichage barre de minerais (2)
            destRect.w *= planets[i].currentOre / planets[i].maxOre;

            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderFillRect(renderer, &destRect);
        }
    }
}
