#include "camera.h"
#include "config.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>

Camera camera;

void initCamera() {
    camera.rect.x = SCREEN_WIDTH / 2;
    camera.rect.y = SCREEN_HEIGHT / 2;
    camera.rect.h = SCREEN_HEIGHT;
    camera.rect.w = SCREEN_WIDTH;
    camera.scale = 1.0f;
    printf("Camera : x=%.2d, y=%.2d, scale=%.2f\n", camera.rect.x, camera.rect.y , camera.scale);
}

void updateCamera(float dx, float dy, float zoomFactor) {
    camera.rect.x += dx / camera.scale;
    camera.rect.y += dy / camera.scale;
    if ((zoomFactor > 1 && camera.scale < 7) || (zoomFactor < 1 && camera.scale > 0.4)) {
        camera.scale *= zoomFactor;
        camera.rect.h *= zoomFactor;
        camera.rect.w *= zoomFactor;
    }
}
