#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "config.h"

Camera camera;

void initCamera(void) {
    camera.rect.x = 0;
    camera.rect.y = 0;
    camera.rect.h = SCREEN_HEIGHT;
    camera.rect.w = SCREEN_WIDTH;
    camera.scale = 1.0;
    printf("Camera : x=%.2d, y=%.2d, scale=%.2f\n", camera.rect.x, camera.rect.y , camera.scale);
}

void zoomCamera(float zoomFactor) {  // Le zoom se bloque lorsqu'un depassement est detecte (temporaire)
    if (zoomFactor < 1 && camera.scale > 0.3) {  // Limiter le zoom
        if ((camera.rect.x + SCREEN_WIDTH / 2.f) * camera.scale * zoomFactor >= SCREEN_WIDTH / 2.f && 
            (camera.rect.y + SCREEN_HEIGHT / 2.f) * camera.scale * zoomFactor >= SCREEN_HEIGHT / 2.f &&
            (camera.rect.x + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale * zoomFactor <= -SCREEN_WIDTH / 2.f &&
            (camera.rect.y + SCREEN_HEIGHT / 2.f - MAP_SIZE) * camera.scale * zoomFactor <= -SCREEN_HEIGHT / 2.f) {
            camera.scale *= zoomFactor;
            camera.rect.h *= zoomFactor;
            camera.rect.w *= zoomFactor;
        }
    } else if (zoomFactor > 1 && camera.scale < 6) {  // Limiter le zoom
        camera.scale *= zoomFactor;
        camera.rect.h *= zoomFactor;
        camera.rect.w *= zoomFactor;
    }
}

void translateCamera(float dx, float dy) {
    if ((camera.rect.x + dx + SCREEN_WIDTH / 2.f) * camera.scale >= SCREEN_WIDTH / 2.f && 
        (camera.rect.y + dy + SCREEN_HEIGHT / 2.f) * camera.scale >= SCREEN_HEIGHT / 2.f &&
        (camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale <= -SCREEN_WIDTH / 2.f && 
        (camera.rect.y + dy + SCREEN_HEIGHT / 2.f - MAP_SIZE) * camera.scale <= -SCREEN_HEIGHT / 2.f) {  // Eviter que la camera sorte de la map
        camera.rect.x += dx;
        camera.rect.y += dy;
    }
}
