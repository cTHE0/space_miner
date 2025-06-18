#include "planet.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "camera.h"
#include "config.h"
#include "solar_system.h"
#include "renderer.h"
#include "tools.h"
#include "ore.h"
#include "build.h"
#include "ship.h"


static Uint32 lastPlanetUpdateTime = 0;
static int nbSolarSystems = 0;

int getNbSolarSystems(void) {
    return nbSolarSystems;
}

void initPlanets(Planet **planets, int planetCount) {
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
        nbSolarSystems ++;
    }

    // Initialisation des batiments et du systeme de rotation des planetes
    SDL_Point solarCenter = {0, 0};
    for (int i = 0; i < planetCount; i++) {
        Planet *p = &(*planets)[i];

        // Initialise les batiments de cette planete
        initBuildsPlanet(p);

        // Définir le centre solaire si c'est le Soleil
        if (p->planetType == SUN) {
            solarCenter.x = p->x;
            solarCenter.y = p->y;
        } else {
            p->orbitCenterX = solarCenter.x;
            p->orbitCenterY = solarCenter.y;
            p->orbitRadius = sqrt(carre(p->x - solarCenter.x) + carre(p->y - solarCenter.y));
            p->orbitAngleDeg = atan2(p->y - solarCenter.y, p->x - solarCenter.x) * 180.0 / M_PI;
            p->orbitSpeedDeg = 0.5 + (rand() % 10) * 0.05; // entre 0.5 a 1
        }
    }
}

void updatePlanets(Planet *planets, Ship *ships, int shipCount, int planetCount) {
    if (SDL_GetTicks() < lastPlanetUpdateTime + WAIT_TIME_PLANET) {
        return;
    }

    lastPlanetUpdateTime += WAIT_TIME_PLANET;

    // Mise a jour des rotations des planètes
    for (int i = 0; i < planetCount; i++) {
        if (planets[i].planetType != SUN) {
            // Mise à jour de l'angle
            planets[i].orbitAngleDeg += planets[i].orbitSpeedDeg;
            if (planets[i].orbitAngleDeg > 360.0)
                planets[i].orbitAngleDeg -= 360.0;

            // Recalcule la position à partir du centre et du rayon
            double angle = planets[i].orbitAngleDeg * M_PI / 180.0;
            planets[i].x = planets[i].orbitCenterX + planets[i].orbitRadius * cos(angle);
            planets[i].y = planets[i].orbitCenterY + planets[i].orbitRadius * sin(angle);
        }
    }

    // Mise a jour des fusees posees dessus
    for (int i = 0; i < shipCount; i++) {
        if (ships[i].state == WAITING_ON_TARGET && ships[i].target.type == SPOT_PLANET && planets[ships[i].target.planet->id].planetType != SUN) {
            ships[i].x = planets[ships[i].target.planet->id].x + planets[ships[i].target.planet->id].radius * cos(ships[i].angleWithPlanet) - ships[i].w / 2;
            ships[i].y = planets[ships[i].target.planet->id].y + planets[ships[i].target.planet->id].radius * sin(ships[i].angleWithPlanet) - ships[i].w / 2;
        }
        else if (ships[i].state == WAITING_ON_BASE && ships[i].base.type == SPOT_PLANET && planets[ships[i].base.planet->id].planetType != SUN) {
            ships[i].x = planets[ships[i].base.planet->id].x + planets[ships[i].base.planet->id].radius * cos(ships[i].angleWithPlanet) - ships[i].w / 2;
            ships[i].y = planets[ships[i].base.planet->id].y + planets[ships[i].base.planet->id].radius * sin(ships[i].angleWithPlanet) - ships[i].w / 2;
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
            int idPicture = (planets[i].planetType == SUN) ? 9 : (generateRandNb8(currentSeed, i) % 7 + 1);
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
        
        displayBuildsAroundPlanet(&planets[i], 3, imageTextures[9], (SDL_Point){60, 60});
    }
}

void destroyPlanets(Planet *planets) {
    free(planets);
}
