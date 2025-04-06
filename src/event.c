#include <SDL2/SDL.h>
#include "event.h"
#include "camera.h"
#include "config.h"
#include "landing_page.h"
#include "window.h"
#include "ship.h"
#include "renderer.h"

void handleEvents(GameState *state, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    static int lastMouseX, lastMouseY;
    static int dragging_camera = 0;
    static int click = 0;
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
                    dragging_camera = 1;
                    click = 1;
                    lastMouseX = event.button.x;
                    lastMouseY = event.button.y;

                    SDL_Point mouse = {event.button.x, event.button.y};
                    if (!SDL_PointInRect(&mouse, &windowRect)) {
                        windowOpened = NO_WINDOW;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (click) {
                        openWindowGestion(lastMouseX, lastMouseY, ships, shipCount, planets, planetCount); // On a cliqué sur un ship donc on ouvre une nouvelle fenêtre
                    }
                    dragging_camera = 0;
                    click = 0;
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging_camera) {
                    int dx = (event.motion.x - lastMouseX);
                    int dy = (event.motion.y - lastMouseY);
                    translateCamera(-dx / camera.scale, -dy / camera.scale);
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                    click = 0;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *state = QUIT;
                        break;
                    case SDLK_o:
                        zoomCamera(1.1f);
                        click = 0;
                        break;
                    case SDLK_p:
                        zoomCamera(1 / 1.1f);
                        click = 0;
                        break;
                    case SDLK_LEFT:
                        translateCamera(-STEP_TRANSLATION, 0);
                        click = 0;
                        break;
                    case SDLK_RIGHT:
                        translateCamera(STEP_TRANSLATION, 0);
                        click = 0;
                        break;
                    case SDLK_UP:
                        translateCamera(0, -STEP_TRANSLATION);
                        click = 0;
                        break;
                    case SDLK_DOWN:
                        translateCamera(0, STEP_TRANSLATION);
                        click = 0;
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
