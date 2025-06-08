#include "planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "solar_system.h"
#include "renderer.h"
#include "tools.h"
#include "ore.h"
#include "build.h"

float theta_deg = 0;
Uint32 lastPlanetUpdateTime = 0;


void updatePlanet(Planet *planets) {
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime > lastPlanetUpdateTime + SPRITE_SHEETS_DELAY/15) {
        lastPlanetUpdateTime = currentTime;

        for (int i = 0; i < INIT_PLANET_COUNT; i++) {
            if (planets[i].planetType != SUN) {
                // Mise à jour de l'angle
                planets[i].orbit_angle_deg += planets[i].orbit_speed_deg;
                if (planets[i].orbit_angle_deg >= 360.0)
                    planets[i].orbit_angle_deg -= 360.0;

                // Recalcule la position à partir du centre et du rayon
                double angle_rad = planets[i].orbit_angle_deg * M_PI / 180.0;
                planets[i].x = (int)(planets[i].orbit_center_x + planets[i].orbit_radius * cos(angle_rad));
                planets[i].y = (int)(planets[i].orbit_center_y + planets[i].orbit_radius * sin(angle_rad));
            }
        }
    }
}


void generatePlanets(Planet **planets, int planetCount) {
    // Allocation du tableau de planètes
    *planets = malloc(planetCount * sizeof(Planet));
    if (*planets == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation mémoire pour les planètes\n");
        exit(EXIT_FAILURE);
    }

    // Génération des planètes via la fonction système solaire
    int nbEntityGenerated = 0;
    while (nbEntityGenerated < planetCount) {
        solarSystemCoordinator(&nbEntityGenerated, *planets, planetCount);
    }

    SDL_Point solar_center = {0, 0};

    for (int i = 0; i < planetCount; i++) {
        Planet *p = &(*planets)[i];

        // Définir le centre solaire si c'est le Soleil
        if (p->planetType == SUN) {
            solar_center.x = p->x;
            solar_center.y = p->y;
        }

        // Initialisation des builds
        for (int j = 0; j < BUILD_TYPE_COUNT; j++) {
            p->builds[j].level = 0;
            if (j < ORE_TYPE_COUNT) {
                p->builds[j].type = ORE_STORE;
            } else if (j < 2 * ORE_TYPE_COUNT) {
                p->builds[j].type = ORE_MINE;
            }
        }

        p->builds[10].type = DEFENCE_TOWER;
        p->builds[11].type = FACTORY;

        // Initialisation des tanks
        p->builds[0].data.tank = (Compartment){FUEL, 0, 0, 0, 0, 200, 500, 20};
        p->builds[1].data.tank = (Compartment){ORE1, 0, 0, 0, 0, 200, 500, 20};
        p->builds[2].data.tank = (Compartment){ORE2, 0, 0, 0, 0, 200, 500, 20};
        p->builds[3].data.tank = (Compartment){ORE3, 0, 0, 0, 0, 200, 500, 20};
        p->builds[4].data.tank = (Compartment){ORE4, 0, 0, 0, 0, 200, 500, 20};

        // Initialisation de la production
        for (int j = 5; j <= 9; j++) {
            p->builds[j].data.production_speed = 0;
        }
        p->builds[10].data.damages = 0;
        p->builds[11].data.production_speed = 0;

        // Initialisation de l'orbite si ce n'est pas le Soleil
        if (p->planetType != SUN) {
            double dx = p->x - solar_center.x;
            double dy = p->y - solar_center.y;

            p->orbit_center_x = solar_center.x;
            p->orbit_center_y = solar_center.y;
            p->orbit_radius = sqrt(dx * dx + dy * dy);
            p->orbit_angle_deg = atan2(dy, dx) * 180.0 / M_PI;
            p->orbit_speed_deg = 0.01 + (rand() % 5) * 0.01; // entre 0.1 et 0.2
        }
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
