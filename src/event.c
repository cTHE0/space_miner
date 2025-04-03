#include <SDL2/SDL.h>
#include "event.h"
#include "camera.h"
#include "config.h"
#include "landing_page.h"
#include "ship_window.h"
#include "ship.h"

void handleEvents(GameState *state, int ship_count, Ship *ships) {
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

                    openCloseShipWindowsGestion(lastMouseX, lastMouseY, ship_count, ships); // On a cliqué sur un ship donc on ouvre une nouvelle fenêtre
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
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;

                    int which_window_is_selected = whichWindow(lastMouseX, lastMouseY);
                    if (which_window_is_selected == -1) { //On n'a pas cliqué sur une fenêtre mais sur la map
                        translateCamera(-dx, -dy);
                    }
                    else { // On a cliqué sur la fenêtre en position which_window_is_selected
                        moveWindow(dx*camera.scale, dy*camera.scale, which_window_is_selected);
                    }
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
                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
    }
}
