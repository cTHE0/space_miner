#include "event.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"
#include "camera.h"
#include "config.h"
#include "window.h"
#include "landing_page.h"
#include "basic_ship_window.h"

static SDL_Point mouse;

SDL_Point getMouseCoordinates(void) {
    return mouse;
}

void handleEvents(SDL_Texture **textTextures, TTF_Font **fonts, GameState *state, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    static SDL_Point lastMouse;
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
                    lastMouse = (SDL_Point){event.button.x, event.button.y};
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (click) { // Ou a-t-on lache le clic gauche ?
                        openWindowGestion(textTextures, fonts, lastMouse, ships, shipCount, planets, planetCount);
                    }
                    dragging_camera = 0;
                    click = 0;
                }
                break;

            case SDL_MOUSEMOTION:
                mouse = (SDL_Point){event.button.x, event.button.y};
                if (dragging_camera) {
                    int dx = (event.motion.x - lastMouse.x);
                    int dy = (event.motion.y - lastMouse.y);
                    translateCamera(-dx / getCameraScale(), -dy / getCameraScale());
                    lastMouse = (SDL_Point){event.motion.x, event.motion.y};
                    click = 0;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (getWindowType() != NO_WINDOW) {
                            changeWindowType(NO_WINDOW);
                        } else {
                            *state = QUIT;
                        }
                        click = 0;
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
