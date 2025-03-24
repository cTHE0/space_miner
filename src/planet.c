#include "planet.h"
#include "camera.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void generatePlanets(Planet **planets, int count) {
    Planet* planets_temp = realloc(*planets, count*sizeof(Planet));
    if (planets_temp == NULL) {
        printf("Erreur d'allocation mémoire (ou 0 planètes)!\n");
        free(*planets);
        *planets = NULL;  // Éviter un accès à une mémoire libérée
        return;  // Sortir de la fonction pour éviter d'utiliser *planets après un échec
    }
    *planets = planets_temp;

    for (int i = 0; i < count; i++) {
        (*planets)[i].x = rand() % 1000;
        (*planets)[i].y = rand() % 500;
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
        float screenX = (planets[i].x - camera.x) * camera.scale;
        float screenY = (planets[i].y - camera.y) * camera.scale;
        float screenRadius = planets[i].radius * camera.scale;

        if (screenX + screenRadius < 0 || screenX - screenRadius > 1400 || 
            screenY + screenRadius < 0 || screenY - screenRadius > 900) {
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
