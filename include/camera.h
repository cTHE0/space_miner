#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Rect rect; // rect représentant partie de la map à afficher (référentiel map)
    float scale;     // Zoom (1.0 = normal, >1 = zoom avant, <1 = zoom arrière)
} Camera;

extern Camera camera;

void initCamera();
void updateCamera(float dx, float dy, float zoomFactor);

#endif
