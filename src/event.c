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
#include "place.h"


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
                if ((getCameraMode() == FOLLOW_SHIP || getCameraMode() == FOLLOW_PLANET) && 
                    getWindowType() != PLANET_WINDOW && 
                    getWindowType() != SHIP_WINDOW && 
                    getWindowType() != BASIC_SHIP_WINDOW) {
                    changeCameraMode(NORMAL);
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
                    changeCameraMode(NORMAL);
                    if (getWindowType() == BASIC_SHIP_WINDOW && getButtonSelected() == NO_BUTTON) {
                        setWindowType(NO_WINDOW);
                    }
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (getWindowType() != NO_WINDOW) {
                            setWindowType(NO_WINDOW);
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
                    
                    /*---------Partie pr développement------------*/
                    case SDLK_1:
                        setSign();
                        break;
                    case SDLK_x:
                        modifEmp(1, 0, 0, 0);
                        break;
                    case SDLK_y:
                        modifEmp(0, 1, 0, 0);
                        break;
                    case SDLK_w:
                        modifEmp(0, 0, 1, 0);
                        break;
                    case SDLK_h:
                        modifEmp(0, 0, 0, 1);
                        break;
                    case SDLK_s:
                        setScaleDev();
                        break;
                    /*---------Partie pr développement------------*/
                    
                    default:
                        break;
                }
                break;
            
            default:
                break;
        }
    }
}
