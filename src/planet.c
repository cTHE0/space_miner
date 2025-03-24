#include "planet.h"
#include "camera.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void generatePlanets(Planet **planets, int count) {
    *planets = malloc(count * sizeof(Planet));

    if (*planets == NULL) {
        printf("Erreur d'allocation mémoire (ou 0 planètes)!\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *planets après un échec
    }

    for (int i = 0; i < count; i++) {
        (*planets)[i].x = rand() % SCREEN_WIDTH;
        (*planets)[i].y = rand() % SCREEN_HEIGHT;
        (*planets)[i].radius = 20 + rand() % 10;  // Taille entre 20 et 30
        printf("Planète %d : (%.2f, %.2f)\n", i, (*planets)[i].x, (*planets)[i].y);
    }
    
}

LODLevel getLOD(float screenRadius) {
    if (screenRadius > 15) return LOD_HIGH;
    if (screenRadius > 5) return LOD_MEDIUM;
    return LOD_LOW;
}

void renderPlanets(SDL_Renderer *renderer, Planet *planets, int count) {
    for (int i = 0; i < count; i++) {
        //(screenX, screenY) = coordonnées écran
        //(planets[i].x, planets[i].y) = coordonnées monde
        float screenX = (planets[i].x - camera.x) * camera.scale + SCREEN_WIDTH / 2;
        float screenY = (planets[i].y - camera.y) * camera.scale + SCREEN_HEIGHT / 2;
        float screenRadius = planets[i].radius * camera.scale;

        if (screenX + screenRadius < 0 || screenX - screenRadius > SCREEN_WIDTH || 
            screenY + screenRadius < 0 || screenY - screenRadius > SCREEN_HEIGHT) {
            continue;
        }

        LODLevel lod = getLOD(screenRadius);

        switch (lod) {
            case LOD_HIGH:
                filledCircleRGBA(renderer, (int)screenX, (int)screenY, (int)screenRadius, 255, 255, 255, 255);
                break;
            case LOD_MEDIUM:
                circleRGBA(renderer, (int)screenX, (int)screenY, (int)screenRadius, 255, 255, 255, 255);
                break;
            case LOD_LOW:
                pixelRGBA(renderer, (int)screenX, (int)screenY, 255, 255, 255, 255);
                break;
        }
    }
}
