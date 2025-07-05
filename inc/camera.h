#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"


typedef enum {NORMAL, FOLLOW_SHIP, FOLLOW_PLANET} CameraMode;

typedef struct {
    SDL_Rect rect; // rect representant partie de la map a afficher (referentiel map)
    float scale;  // Zoom (1.0 = normal, >1 = zoom avant, <1 = zoom arriere)
    int last_object_selected;
} Camera;


void initCamera(Planet *planets);

void updateCameraFollow(Ship *ships, Planet *planets);

SDL_Rect getCameraRect(void);
CameraMode getCameraMode(void);
int getCameraLastObjectSelected(void);
float getCameraScale(void);

void setCenterCamera(SDL_Point newCenter);
void setCameraMode(CameraMode newCameraMode);
void setCameraLastObjectSelected(int lastObjectSelected);

void zoomCamera(float zoomFactor);
void translateCamera(float dx, float dy);


#endif
