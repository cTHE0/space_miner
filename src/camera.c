#include "camera.h"
#include "config.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>

Camera camera;

void initCamera(void) {
    camera.rect.x = 0;
    camera.rect.y = 0;
    camera.rect.h = SCREEN_HEIGHT;
    camera.rect.w = SCREEN_WIDTH;
    camera.scale = 1.0;
    printf("Camera : x=%.2d, y=%.2d, scale=%.2f\n", camera.rect.x, camera.rect.y , camera.scale);
}

void zoomCamera(float zoomFactor) {
    if (zoomFactor < 1 && camera.scale > 0.5) {  // Limiter le zoom
        float dx = STEP_TRANSLATION, dy = STEP_TRANSLATION;  // Adapter en fonction de camera.scale

        int left = (camera.rect.x + dx + camera.rect.h * (zoomFactor - 1) + SCREEN_WIDTH / 2.f) * camera.scale < SCREEN_WIDTH / 2.f, 
            up = (camera.rect.y + dy + camera.rect.w * (zoomFactor - 1) + SCREEN_HEIGHT / 2.f) * camera.scale < SCREEN_HEIGHT / 2.f, 
            right = (MAP_SIZE - camera.rect.x * camera.scale - SCREEN_WIDTH / 2.f) * camera.scale < SCREEN_WIDTH / 2.f,
            down = (MAP_SIZE - camera.rect.y * camera.scale - SCREEN_HEIGHT / 2.f) * camera.scale < SCREEN_HEIGHT;

        // Eviter que la camera sorte de la map (raisonner avec renderPlanets)
        if (left) {
            camera.rect.x += dx;
            if (up) {
                camera.rect.y += dy;
            } else if (down) {
                camera.rect.y -= dy;
            }
        } else if (right) {
            camera.rect.x -= dx;
            if (up) {
                camera.rect.y += dy;
            } else if (down) {
                camera.rect.y -= dy;
            }
        } else if (up) {  // Condition (!left && !right) implicite 
            camera.rect.y += dy;
        } else if (down) {  // Condition (!left && !right) implicite 
            camera.rect.y -= dy;
        } else {  // Cas ou la camera ne risque pas de depasser
            camera.scale *= zoomFactor;
            camera.rect.h *= zoomFactor;
            camera.rect.w *= zoomFactor;
        }
    } else if (zoomFactor > 1 && camera.scale < 6) {
            camera.scale *= zoomFactor;
            camera.rect.h *= zoomFactor;
            camera.rect.w *= zoomFactor;
    }
}

void translateCamera(float dx, float dy) {
    if ((camera.rect.x + dx + SCREEN_WIDTH / 2.f) * camera.scale >= SCREEN_WIDTH / 2.f && 
        (camera.rect.y + dy + SCREEN_HEIGHT / 2.f) * camera.scale >= SCREEN_HEIGHT / 2.f &&
        (MAP_SIZE - camera.rect.x - dx - SCREEN_WIDTH / 2.f) * camera.scale >= SCREEN_WIDTH / 2.f && 
        (MAP_SIZE - camera.rect.y - dy - SCREEN_HEIGHT / 2.f) * camera.scale >= SCREEN_HEIGHT / 2.f) {  // Eviter que la camera sorte de la map
        camera.rect.x += dx;
        camera.rect.y += dy;
    }
}
