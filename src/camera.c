#include "camera.h"
#include "config.h"
#include <stdio.h>

Camera camera;

void initCamera() {
    camera.x = SCREEN_WIDTH / 2;
    camera.y = SCREEN_HEIGHT / 2;
    camera.scale = 1.0f;
    printf("Camera : x=%.2f, y=%.2f, scale=%.2f\n", camera.x, camera.y , camera.scale);
}

void updateCamera(float dx, float dy, float zoomFactor) {
    camera.x += dx / camera.scale;
    camera.y += dy / camera.scale;
    if (zoomFactor > 1 && camera.scale < 7) {
        camera.scale *= zoomFactor;
    } else if (zoomFactor < 1 && camera.scale > 0.4) {
        camera.scale *= zoomFactor;
    }
}
