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
}

void zoomCamera(float zoomFactor) {
    if (zoomFactor < 1) {  // Limiter le zoom
        float dx1 = camera.rect.x + (1 - 1 / camera.scale / zoomFactor) * SCREEN_WIDTH / 2.f, 
              dy1 = camera.rect.y + (1 - 1 / camera.scale / zoomFactor) * SCREEN_HEIGHT / 2.f,
              dx2 = camera.rect.x - MAP_SIZE + (1 + 1 / camera.scale / zoomFactor) * SCREEN_WIDTH / 2.f,
              dy2 = camera.rect.y - MAP_SIZE + (1 + 1 / camera.scale / zoomFactor) * SCREEN_HEIGHT / 2.f;

        int left = (camera.rect.x + dx1 + SCREEN_WIDTH / 2.f) * camera.scale < SCREEN_WIDTH / 2.f, 
            up = (camera.rect.y + dy1 + SCREEN_HEIGHT / 2.f) * camera.scale < SCREEN_HEIGHT / 2.f,
            right = (camera.rect.x + dx2 + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale > -SCREEN_WIDTH / 2.f,
            down = (camera.rect.y + dy2 + SCREEN_HEIGHT / 2.f - MAP_SIZE) * camera.scale > -SCREEN_HEIGHT / 2.f;
        
        if (!(left && right) && !(up && down)) {  // Verifie que l'on a pas atteint deux bords opposes
            if (left) {
                camera.rect.x -= dx1;
            } else if (right) {
                camera.rect.x -= dx2;
            } 
            if (up) {
                camera.rect.y -= dy1;
            } else if (down) {
                camera.rect.y -= dy2;
            }

            // Maintenant que la camera ne risque pas de depasser :
            camera.scale *= zoomFactor;
        }
    } else if (camera.scale < 6) {  // zoomFactor >= 1 implicite 
        camera.scale *= zoomFactor;
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

// Raisonnement correction limite de zoom

// a = (camera.rect.x + SCREEN_WIDTH / 2.f) * camera.scale * zoomFactor - SCREEN_WIDTH / 2.f < 0
// b = (camera.rect.x + SCREEN_WIDTH / 2.f) * camera.scale - SCREEN_WIDTH / 2.f = 0

// delta = a - b = (camera.rect.x + SCREEN_WIDTH / 2.f) * camera.scale * (zoomFactor - 1) 


// Grace a dx, on veut que delta(camera.rect.x = camera.rect.xcamera.rect.x + dx) = 0:

// (camera.rect.x + dx + SCREEN_WIDTH / 2.f) * camera.scale * zoomFactor - SCREEN_WIDTH / 2.f = 0

// dx = SCREEN_WIDTH / 2.f / camera.scale / zoomFactor - SCREEN_WIDTH / 2.f - camera.rect.x




// a = (camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale * zoomFactor + SCREEN_WIDTH / 2.f > 0
// b = (camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale + SCREEN_WIDTH / 2.f = 0

// delta = a - b = (camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale * (zoomFactor - 1) 


// Grace a dx, on veut que delta(camera.rect.x = camera.rect.xcamera.rect.x + dx) = 0:

// (camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale * zoomFactor + SCREEN_WIDTH / 2.f = 0

// dx = -SCREEN_WIDTH / 2.f / camera.scale / zoomFactor + MAP_SIZE - SCREEN_WIDTH / 2.f - camera.rect.x
