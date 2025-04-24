#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "planet.h"

typedef struct {
    SDL_Rect rect; // rect représentant partie de la map à afficher (référentiel map)
    float scale;  // Zoom (1.0 = normal, >1 = zoom avant, <1 = zoom arrière)
} Camera;

void initCamera(Planet *planets);
void zoomCamera(float zoomFactor);
void translateCamera(float dx, float dy);
SDL_Rect getCameraRect(void);
float getCameraScale(void);

#endif
