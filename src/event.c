#include "event.h"
#include "camera.h"
#include <SDL2/SDL.h>

void handleEvents(int *running) {
    static int lastMouseX, lastMouseY;
    static int dragging = 0;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = 0;
                break;
            case SDL_MOUSEWHEEL:
                updateCamera(0, 0, (event.wheel.y > 0) ? 1.1f : 0.9f);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = 1;
                    lastMouseX = event.button.x;
                    lastMouseY = event.button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                if (dragging) {
                    int dx = event.motion.x - lastMouseX;
                    int dy = event.motion.y - lastMouseY;
                    updateCamera(-dx, -dy, 1.0f);
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *running = 0;
                        break;
                    case SDLK_o:
                        updateCamera(0, 0, 1.1f);
                        break;
                    case SDLK_p:
                        updateCamera(0, 0, 0.9f);
                        break;
                    case SDLK_LEFT:
                        updateCamera(-10, 0, 1);
                        break;
                    case SDLK_RIGHT:
                        updateCamera(10, 0, 1);
                        break;
                    case SDLK_UP:
                        updateCamera(0, -10, 1);
                        break;
                    case SDLK_DOWN:
                        updateCamera(0, 10, 1);
                        break;
                }
                break;
        }
    }
}
