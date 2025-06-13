#include "camera.h"

#include <SDL2/SDL.h>
#include "config.h"
#include "window.h"
#include "planet.h"


static Camera camera;
static CameraMode camera_mode = NORMAL;

void initCamera(Planet *planets) {
    camera.rect.x = planets[1].x - SCREEN_WIDTH / 2;
    camera.rect.y = planets[1].y - SCREEN_HEIGHT / 2;
    camera.rect.h = SCREEN_HEIGHT;
    camera.rect.w = SCREEN_WIDTH;
    camera.scale = 0.5;
}

CameraMode getCameraMode() {
    return camera_mode;
}
void changeCameraLastObjectSelected(int lastObjectSelected) {
    camera.last_object_selected = lastObjectSelected;
}

void changeCameraMode(CameraMode new_camera_mode) {
    camera_mode = new_camera_mode;
}

void updateCameraFollow(Ship *ships,Planet *planets) {
    if (camera_mode == FOLLOW_SHIP) {
        setCenterCamera((SDL_Point){ships[camera.last_object_selected].x + ships[camera.last_object_selected].w/2, ships[camera.last_object_selected].y+ ships[camera.last_object_selected].h/2});
    }
    else if (camera_mode == FOLLOW_PLANET) {
        setCenterCamera((SDL_Point){planets[camera.last_object_selected].x + planets[camera.last_object_selected].radius/2, planets[camera.last_object_selected].y + planets[camera.last_object_selected].radius/2});
    }
}

SDL_Rect getCameraRect(void) {
    return camera.rect;
}

float getCameraScale(void) {
    return camera.scale;
}

void setCenterCamera(SDL_Point newCenter) {
    camera.rect.x = newCenter.x - SCREEN_WIDTH / 2;
    camera.rect.y = newCenter.y - SCREEN_HEIGHT / 2;
}

void zoomCamera(float zoomFactor) {
    if ((getWindowType() != NO_WINDOW) && (getWindowType() != BASIC_SHIP_WINDOW)) {
        return;
    }
    
    if (zoomFactor < 1) {  // Limiter le zoom
        float dx1 = camera.rect.x + (1 - 1 / (camera.scale * zoomFactor)) * SCREEN_WIDTH / 2.f, 
              dy1 = camera.rect.y + (1 - 1 / (camera.scale * zoomFactor)) * SCREEN_HEIGHT / 2.f,
              dx2 = camera.rect.x - MAP_SIZE + (1 + 1 / (camera.scale * zoomFactor)) * SCREEN_WIDTH / 2.f,
              dy2 = camera.rect.y - MAP_SIZE + (1 + 1 / (camera.scale * zoomFactor)) * SCREEN_HEIGHT / 2.f;

        int left = (camera.rect.x + dx1 + SCREEN_WIDTH / 2.f) * camera.scale < SCREEN_WIDTH / 2.f, 
            up = (camera.rect.y + dy1 + SCREEN_HEIGHT / 2.f) * camera.scale < SCREEN_HEIGHT / 2.f,
            right = (camera.rect.x + dx2 + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale > -SCREEN_WIDTH / 2.f,
            down = (camera.rect.y + dy2 + SCREEN_HEIGHT / 2.f - MAP_SIZE) * camera.scale > -SCREEN_HEIGHT / 2.f;
        
        if (MAP_SIZE > SCREEN_WIDTH / camera.scale / zoomFactor) {  // Limite de dezoom
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
    if ((getWindowType() != NO_WINDOW) && (getWindowType() != BASIC_SHIP_WINDOW)){
        return;
    }

    if (dx < 0) {
        if ((camera.rect.x + dx + SCREEN_WIDTH / 2.f) * camera.scale >= SCREEN_WIDTH / 2.f) {
            camera.rect.x += dx;
        }
    } else {
        if ((camera.rect.x + dx + SCREEN_WIDTH / 2.f - MAP_SIZE) * camera.scale <= -SCREEN_WIDTH / 2.f) {
            camera.rect.x += dx;
        }
    }

    if (dy < 0) {
        if ((camera.rect.y + dy + SCREEN_HEIGHT / 2.f) * camera.scale >= SCREEN_HEIGHT / 2.f) {
            camera.rect.y += dy;
        }
    } else {
        if ((camera.rect.y + dy + SCREEN_HEIGHT / 2.f - MAP_SIZE) * camera.scale <= -SCREEN_HEIGHT / 2.f) {
            camera.rect.y += dy;
        }
    }
}
