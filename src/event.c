#include <SDL2/SDL.h>
#include "event.h"
#include "camera.h"
#include "config.h"
#include "landing_page.h"
#include "ship_window.h"

void handleEvents(GameState *state) {
    static int lastMouseX, lastMouseY;
    static int dragging = 0;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *state = QUIT;
                break;
                
            case SDL_MOUSEWHEEL:
                zoomCamera((event.wheel.y > 0) ? 1.1f : 1 / 1.1f);
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
                    int dx = (event.motion.x - lastMouseX) / camera.scale;
                    int dy = (event.motion.y - lastMouseY) / camera.scale;
                    translateCamera(-dx, -dy);
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *state = QUIT;
                        break;
                    case SDLK_o:
                        zoomCamera(1.1f);
                        break;
                    case SDLK_p:
                        zoomCamera(1 / 1.1f);
                        break;
                    case SDLK_LEFT:
                        translateCamera(-STEP_TRANSLATION, 0);
                        break;
                    case SDLK_RIGHT:
                        translateCamera(STEP_TRANSLATION, 0);
                        break;
                    case SDLK_UP:
                        translateCamera(0, -STEP_TRANSLATION);
                        break;
                    case SDLK_DOWN:
                        translateCamera(0, STEP_TRANSLATION);
                        break;
                    case SDLK_f:
                        addShipWindows();
                        break;
                    case SDLK_v:
                        deleteShipWindows();
                        break;
                }
                break;
        }
    }
}
