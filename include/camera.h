#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x, y;      // Position caméra dans le monde
    float scale;     // Zoom (1.0 = normal, >1 = zoom avant, <1 = zoom arrière)
} Camera;

extern Camera camera;

void initCamera();
void updateCamera(float dx, float dy, float zoomFactor);

#endif
