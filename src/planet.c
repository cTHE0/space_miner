#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "planet.h"
#include "camera.h"
#include "config.h"
#include "solar_system.h"

void generatePlanets(Planet **planets, int count) {
    *planets = malloc(count * sizeof(Planet));

    generateSolarSystem(planets, MAP_SIZE/2, MAP_SIZE/2, 0, count - 1);

}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, SDL_Texture ***texturePlanet, int count) {
    for (int i = 0; i < count; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point en haut à gauche du rect de la planete
        float screenX = (planets[i].x - camera.rect.x - planets[i].radius - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenY = (planets[i].y - camera.rect.y - planets[i].radius - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * camera.scale;  

        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        if (screenX >= -2 * screenRadius && screenX <= SCREEN_WIDTH && 
            screenY >= -2 * screenRadius && screenY <= SCREEN_HEIGHT + screenRadius) {   // On n'affiche pas les planètes situés en dehors de l'ecran  
            // Affichage planete 
            SDL_RenderCopy(renderer, texturePlanet[5][planets[i].idPicture], NULL, &destRect);

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
