#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"

typedef struct {
    SDL_Rect rect; // rect representant partie de la map a afficher (referentiel map)
    float scale;  // Zoom (1.0 = normal, >1 = zoom avant, <1 = zoom arriere)
    int last_object_selected;
} Camera;

typedef enum {NORMAL, FOLLOW_SHIP, FOLLOW_PLANET} CameraMode;

void changeCameraLastObjectSelected(int lastObjectSelected);
void initCamera(Planet *planets);
CameraMode getCameraMode();
void updateCameraFollow(Ship *ships,Planet *planets);
void setCameraMode(CameraMode new_camera_mode);
void zoomCamera(float zoomFactor);
void translateCamera(float dx, float dy);
SDL_Rect getCameraRect(void);
float getCameraScale(void);
void setCenterCamera(SDL_Point newCenter);

#endif
