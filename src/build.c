#include "build.h"

#include "planet.h"
#include "renderer.h"
#include "camera.h"


void initBuildsPlanet(Planet *planet) {
	// Cree tous les batiments sur cette planete
    for (int i = 0; i < BUILD_TYPE_COUNT; i++) {
        planet->builds[i].level = 0;
        if (i < ORE_TYPE_COUNT) {
            planet->builds[i].type = ORE_STORE;
        } else if (i < 2 * ORE_TYPE_COUNT) {
            planet->builds[i].type = ORE_MINE;
        } else if (i == 10) {
            planet->builds[11].type = DEFENCE_TOWER;
        } else if (i == 11) {
            planet->builds[11].type = FACTORY;
        }
    }

    // Initialisation des reservoirs et des mines
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        planet->builds[i].tank = (Compartment){i, 0, 0, 0, 0, 8000, 10000, 20};
        planet->builds[i + ORE_TYPE_COUNT].productionSpeed = 0;
    }

    // Initialisation de l'usine et de la tourelle de defence
    planet->builds[10].damages = 0;
    planet->builds[11].productionSpeed = 0;
}

void displayBuildsAroundPlanet(Planet *planet, int countBuild, SDL_Texture **imageTextures, SDL_Point texture_dimensions) {
    float scale = getCameraScale();

    // Taille affichée de la planète
    float screenRadius = 43 * planet->radius * scale / 40;

    // Le vrai centre affiché de la planète
    float centerX = (planet->x - getCameraRect().x - SCREEN_WIDTH / 2.0f) * scale + SCREEN_WIDTH / 2.0f;
    float centerY = (planet->y - getCameraRect().y - SCREEN_HEIGHT / 2.0f) * scale + SCREEN_HEIGHT / 2.0f;

    SDL_Point center = {texture_dimensions.x * scale / 2, texture_dimensions.y * scale / 2};

    for (int i = 0; i < countBuild; i++) {
        // Angle autour de la planète
        float angle = i * 2.0f * M_PI / countBuild;
        float angle_deg = angle * 180.0f / M_PI;

        // Position orbitale autour du centre
        float x = centerX + screenRadius * cosf(angle);
        float y = centerY + screenRadius * sinf(angle);

        // Rectangle destination
        SDL_Rect dest = {(int)(x - texture_dimensions.x * scale / 2),
                         (int)(y - texture_dimensions.y * scale / 2),
                         (int)(texture_dimensions.x * scale),
                         (int)(texture_dimensions.y * scale)
                        };

        // Tangente = angle + 90°
        float tangent_angle = angle_deg + 90.0f;

        SDL_RenderCopyEx(renderer, imageTextures[i], NULL, &dest, tangent_angle, &center, SDL_FLIP_NONE);
    }
}
