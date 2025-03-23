#include "planet.h"
#include "camera.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void generatePlanets(Planet *planets, int count) {
    for (int i = 0; i < count; i++) {
        planets[i].x = rand() % 800;  // Assure-toi que les coordonnées sont correctes
        planets[i].y = rand() % 600;
        planets[i].radius = 20 + rand() % 10;  // Taille entre 20 et 30
        printf("Planète %d : (%.2f, %.2f)\n", i, planets[i].x, planets[i].y);
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

        if (screenX + screenRadius < 0 || screenX - screenRadius > 800 || 
            screenY + screenRadius < 0 || screenY - screenRadius > 600) {
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
