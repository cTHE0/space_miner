#include "planet.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "solar_system.h"
#include "renderer.h"
#include "tools.h"
int id;                          // Identifiant de la planet (permet la generation aleatoire
planetTypeEnum planetType;       // Type de planete : soleil, planete gazeuse, planete rocheuse, ...
float x, y;                      // Position du centre de la planete sur la map
float radius;
float maxOre[ORE_TYPE_COUNT];    // Quantité maximale et actuelle de minerais
float currentOre[ORE_TYPE_COUNT];
float regenerationTime;

void generatePoints(Planet *planets, int planetCount, int nbPointsPerLigne) {
    for (int i = 0; i < nbPointsPerLigne * nbPointsPerLigne; i++) {
        planets[planetCount + i].radius = 20;
        planets[planetCount + i].x = (i % nbPointsPerLigne) * 500;
        planets[planetCount + i].y = (i / nbPointsPerLigne) * 500;
        planets[planetCount + i].planetType = POINT;
    }
}



void generatePlanets(Planet **planets, int planetCount, int pointCount, int nbPointsPerLigne) {
    // Allocation du tableau de planetes
    *planets = malloc((planetCount + pointCount) * sizeof(Planet)); // Les points sont gérés comme des planètes

    // Generations des planetes, systeme solaire par systeme solaire
    int nbEntityGenerated = 0;  // Nombre planètes (étoiles incluses) déjà présentes
    while (nbEntityGenerated < planetCount) {
        solarSystemCoordinator(&nbEntityGenerated, *planets, planetCount);
    }
    
    generatePoints(*planets, planetCount, nbPointsPerLigne);
}

void renderPoints(SDL_Texture ***imageTextures, Planet *planets, int planetCount, int pointCount, int nbPointsPerLigne){
    SDL_Rect pointRect;
    for (int i=0; i<nbPointsPerLigne; i++){
        for (int j=0; j<nbPointsPerLigne; j++){
            pointRect = (SDL_Rect){i*500, j*500, 20, 20};
            SDL_RenderCopy(renderer, imageTextures[5][5], NULL, &pointRect);
        }
    }
}

void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount) {
    for (int i = 0; i < planetCount; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point en haut à gauche du rect de la planete
        float screenX = (planets[i].x - getCameraRect().x - planets[i].radius - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenY = (planets[i].y - getCameraRect().y - planets[i].radius - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * getCameraScale();  

        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        if (screenX >= -2 * screenRadius && screenX <= SCREEN_WIDTH && 
            screenY >= -2 * screenRadius && screenY <= SCREEN_HEIGHT + screenRadius) {   // On n'affiche pas les planètes situés en dehors de l'ecran  
            // Affichage planete 
            int idPicture = (planets[i].planetType == SUN) ? 9 : (generateRandNb8(currentSeed, i) % 8 + 1);
            SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &destRect);

            // Affichage barre* de minerais (1)      (*une seule barre, mais representant la valeur totale de minerais !?)
            destRect.h = 5 * getCameraScale();
            destRect.y -= destRect.h + 2 / getCameraScale();

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &destRect);

            // Affichage barre de minerais (2)
            destRect.w *= planets[i].currentOre[0] / planets[i].maxOre[0];

            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderFillRect(renderer, &destRect);
        }
    }
}
