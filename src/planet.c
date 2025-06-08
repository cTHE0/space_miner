#include "planet.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "solar_system.h"
#include "renderer.h"
#include "tools.h"


void generatePlanets(Planet **planets, int planetCount) {
    // Allocation du tableau de planetes
    *planets = malloc(planetCount * sizeof(Planet));

    // Generations des planetes, systeme solaire par systeme solaire
    int nbEntityGenerated = 0;  // Nombre planetes (etoiles incluses) deja presentes
    while (nbEntityGenerated < planetCount) {
        solarSystemCoordinator(&nbEntityGenerated, *planets, planetCount);
    }
}

void renderPlanets(SDL_Texture ***imageTextures, Planet *planets, int planetCount) {
    for (int i = 0; i < planetCount; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point en haut a gauche du rect de la planete
        float screenX = (planets[i].x - getCameraRect().x - planets[i].radius - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenY = (planets[i].y - getCameraRect().y - planets[i].radius - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * getCameraScale();  

        SDL_Rect destRect = {screenX, screenY, 2 * screenRadius, 2 * screenRadius};

        if (screenX >= -2 * screenRadius && screenX <= SCREEN_WIDTH && 
            screenY >= -2 * screenRadius && screenY <= SCREEN_HEIGHT + screenRadius) {   // On n'affiche pas les planetes situes en dehors de l'ecran  
            // Affichage planete 
            int idPicture = (planets[i].planetType == SUN) ? 9 : (generateRandNb8(currentSeed, i) % 8 + 1);
            SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &destRect);

            /*
            // Affichage barre* de minerais (1)      (*une seule barre, mais representant la valeur totale de minerais !?)
            destRect.h = 5 * getCameraScale();
            destRect.y -= destRect.h + 2 / getCameraScale();

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &destRect);

            // Affichage barre de minerais (2)
            destRect.w *= planets[i].currentOre[0] / planets[i].maxOre[0];

            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderFillRect(renderer, &destRect);
            */
        }
        
        displayBuildAroundPlanet(planets[i], 3, imageTextures[9], (SDL_Point){60, 60});
    }
}

void displayBuildAroundPlanet(Planet planet, int nb_build, SDL_Texture **build_textures_list, SDL_Point texture_dimensions) {
    float scale = getCameraScale();
    SDL_Rect camera = getCameraRect();

    // Taille affichée de la planète
    float screenRadius = 43* planet.radius * scale / 40;

    // Le vrai centre affiché de la planète
    float centerX = (planet.x - camera.x - SCREEN_WIDTH / 2.0f) * scale + SCREEN_WIDTH / 2.0f;
    float centerY = (planet.y - camera.y - SCREEN_HEIGHT / 2.0f) * scale + SCREEN_HEIGHT / 2.0f;

    SDL_Point center = {
        (int)(texture_dimensions.x * scale / 2),
        (int)(texture_dimensions.y * scale / 2)
    };

    for (int i = 0; i < nb_build; i++) {
        // Angle autour de la planète
        float angle_rad = i * 2.0f * M_PI / nb_build;
        float angle_deg = angle_rad * 180.0f / M_PI;

        // Position orbitale autour du centre
        float x = centerX + screenRadius * cosf(angle_rad);
        float y = centerY + screenRadius * sinf(angle_rad);

        // Rectangle destination
        SDL_Rect dest = {
            (int)(x - texture_dimensions.x * scale / 2),
            (int)(y - texture_dimensions.y * scale / 2),
            (int)(texture_dimensions.x * scale),
            (int)(texture_dimensions.y * scale)
        };

        // Tangente = angle + 90°
        float tangent_angle = angle_deg + 90.0f;

        SDL_RenderCopyEx(renderer, build_textures_list[i], NULL, &dest, tangent_angle, &center, SDL_FLIP_NONE);
    }
}



void destroyPlanets(Planet *planets, int planetCount) {
    free(planets);
}
