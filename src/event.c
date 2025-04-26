#include "event.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"
#include "camera.h"
#include "config.h"
#include "window.h"
#include "landing_page.h"


void handleEvents(SDL_Texture **textTextures, TTF_Font **fonts, GameState *state, Ship *ships, int shipCount, Planet *planets, int planetCount) {
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

                    switch (getWindowType()) {
                        case SHIP_WINDOW:
                            if (!clickOnWindow((SDL_Point){event.button.x, event.button.y})){
                                changeWindowType(NO_WINDOW);
                            }
                            break;
                        case PLANET_WINDOW:
                            if (!clickOnWindow((SDL_Point){event.button.x, event.button.y})){
                                changeWindowType(NO_WINDOW);
                            }
                            break;
                        default:
                            break;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (click) {
                        openWindowGestion(textTextures, fonts, (SDL_Point){lastMouseX, lastMouseY}, ships, shipCount, planets, planetCount); // On a cliqué sur un ship donc on ouvre une nouvelle fenêtre
                    }
                    dragging_camera = 0;
                    click = 0;
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging_camera) {
                    int dx = (event.motion.x - lastMouseX);
                    int dy = (event.motion.y - lastMouseY);
                    translateCamera(-dx / getCameraScale(), -dy / getCameraScale());
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                    click = 0;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (getWindowType() != NO_WINDOW) {
                            changeWindowType(NO_WINDOW);
                        }
                        else {
                            *state = QUIT;
                        }
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
