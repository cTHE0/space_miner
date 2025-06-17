#include "build.h"

#include "planet.h"
#include "ship.h"
#include "renderer.h"
#include "camera.h"
#include "mine.h"
#include "config.h"


static Uint32 lastBuildUpdateTime = 0;


void initBuildsPlanet(Planet *planet) {
	// Cree tous les reservoirs sur les planetes
    for (int i = 0; i < BUILD_TYPE_COUNT; i++) {
        if (i < ORE_TYPE_COUNT) {
            planet->builds[i].type = ORE_STORE;
            planet->builds[i].level = 1;
            planet->builds[i].tank = (Compartment){i, 0, 0, 0, 0, 1, 8000, 10000, 20};
        } else if (i < 5 + ORE_TYPE_COUNT) {
            planet->builds[i].type = ORE_MINE;
            planet->builds[i].level = 0;
            planet->builds[i].mine = (Mine){i - 5 , 1000};
        } else if (i == 10) {
            planet->builds[i].type = FACTORY;
            planet->builds[i].level = 0;
        } else if (i == 11) {
            planet->builds[i].type = DEFENCE_TOWER;
            planet->builds[i].level = 0;
        }
    }
}

void updateBuilds(Planet *planets, int planetCount) {
    if (SDL_GetTicks() < lastBuildUpdateTime + REFRESH_TIME_BUILDS) {
        return;
    }

    lastBuildUpdateTime += REFRESH_TIME_BUILDS;

    for (int i = 0; i < planetCount; i++) {
        for (int j = 0; j < BUILD_TYPE_COUNT; j++) {
            if (planets[i].builds[j].type == NOTHING || planets[i].builds[j].level == 0) {
                continue;
            } 

            switch (planets[i].builds[j].type) {
                case ORE_STORE:  // Tout est gere dans ship.c
                    break;

                case ORE_MINE:
                    updateBuildMine(planets[i].builds, &planets[i].builds[j].mine, planets[i].abundance[planets[i].builds[j].mine.ore]);
                    break;

                case DEFENCE_TOWER:
                    break;

                case FACTORY:
                    break;

                default:
                    break;
            }
        }
    }
}

void updateBuildMine(Build *builds, Mine *mine, int abundance) {
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        if (builds[i].type == ORE_STORE && builds[i].tank.ore == mine->ore) {  // Si l'on a trouve le bon reservoir, acceuillant les bons minerais...
            builds[i].tank.currentCapacity += mine->productivity * (abundance / 100.f);

            if (builds[i].tank.currentCapacity > builds[i].tank.maxCapacity) {
                builds[i].tank.currentCapacity = builds[i].tank.maxCapacity;
            }
            return;
        }
    }

    printf("Pas d'emplacement de stockage trouve pour vider la mine (build.c)\n");
    return;
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
