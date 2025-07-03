#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "camera.h"
#include "basic_ship_window.h"
#include "ship_window.h"
#include "planet_window.h"
#include "info_view.h"
#include "side_bar_window.h"
#include "pause_window.h"
#include "tools.h"
#include "event.h"
#include "assets_gestion.h"
#include "asteroid.h"
#include "tile.h"
#include "enemy.h"
#include "landing_page.h"


static int selectionMode = 0;
static SDL_Point centerSelectionCircle;

static Window windowInfo = {NO_WINDOW, 0};

static const SDL_Rect windowRect = {(1 - 0.8) * SCREEN_WIDTH / 2., 
                                    (1 - 0.8) * SCREEN_HEIGHT / 2., 
                                    SCREEN_WIDTH * 0.8,
                                    SCREEN_HEIGHT * 0.8};


int clickOnWindow(SDL_Point mouse) {
    return SDL_PointInRect(&mouse, &windowRect);
}

WindowType getWindowType(void) {
    return windowInfo.type;
}

int getWindowId(void) {
    return windowInfo.id;
}

void setWindowType(WindowType newType) {
    windowInfo.type = newType;
}

void setWindowId(int newId) {
    windowInfo.id = newId;
}

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, GameState *gameState, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    switch (getWindowType()) {

        case SIDE_BAR_WINDOW:
        case NO_WINDOW:
            if (clickSideBar(mouse)) {
                Mix_PlayChannel(1, sounds[7], 0);
                selectionMode = 0;
            } else if (clickOnShip(textTextures, fonts, ships, shipCount, planets, mouse)) {
                selectionMode = 0;
                Mix_PlayChannel(1, sounds[4], 0);
            } else if (clickOnPlanet(textTextures, fonts, planets, planetCount, mouse)) {
                selectionMode = 0;
                Mix_PlayChannel(1, sounds[3], 0);
            }

            // Si rien n'a ete clique, on s'occupe du mode 'selection'
            else if (selectionMode == 0) {
                selectionMode = 1;
                centerSelectionCircle = (SDL_Point){mouse.x, mouse.y};
            } else if (selectionMode == 1) {
                selectionMode = 0;
                objetInSelectionCircle(textTextures, fonts, sounds, ships, shipCount, planets, planetCount);
            }

            break;

        case BASIC_SHIP_WINDOW:
            basicShipWindowGestion(textTextures, fonts, sounds, ships, shipCount, planets, planetCount, mouse);
            break;

        case SHIP_WINDOW:
            shipWindowGestion(textTextures, fonts, sounds, &ships[getWindowId()], planets, mouse);
            break;

        case PLANET_WINDOW:
            planetWindowGestion(textTextures, fonts, sounds, planets, mouse);
            break;

        case PAUSE_WINDOW:
            pauseWindowGestion(textTextures, fonts, sounds, gameState, mouse, ships, shipCount, planets, planetCount);
            break;

        default:
            break;
    }
}

int whichShipIsClicked(Ship *ships, int shipCount, SDL_Point mouse) {
    // Renvoie l'id du ship selectionne ou -1 sinon
    for (int i = 0; i < shipCount; i++) {
        if (SDL_PointInRect(&mouse, &ships[i].destRect)) {
            return i;
        }
    }
    return -1;
}

int clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, Planet *planets, SDL_Point mouse) {
    int id = whichShipIsClicked(ships, shipCount, mouse); // id du ship selectionne ou -1 sinon

    if (id != -1) {  // Si un ship a etait clique...
        setWindowId(id);
        setWindowType(BASIC_SHIP_WINDOW);
        initBasicShipWindow(textTextures, fonts, &ships[id], ships, planets);
        setCameraLastObjectSelected(id);
        setCameraMode(FOLLOW_SHIP);
    }

    return id != -1;
}

int whichPlanetIsClicked(Planet *planets, int planetCount, SDL_Point mouse) {
    // Renvoie la planete sur lequelle user a clique ou -1 sinon
    for (int i = 0; i < planetCount; i++) {
        //(screenX, screenY) = coordonnees sur l'ecran physique, du point au milieu de la planete
        // (planets[i].x, planets[i].y) = coordonnees sur la map
        float screenX = (planets[i].x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
        float screenY = (planets[i].y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;  
        float screenRadius = planets[i].radius * getCameraScale();  
        if (fabs(screenX - mouse.x) < screenRadius && fabs(screenY - mouse.y) < screenRadius) {
            return i;
        }
    }
    return -1;
}

int clickOnPlanet(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, int planetCount, SDL_Point mouse) {
    int id = whichPlanetIsClicked(planets, planetCount, mouse);

    if (id != -1) {
        setWindowId(id);
        setWindowType(PLANET_WINDOW);
        initPlanetWindow(textTextures, fonts, planets);
        setCameraLastObjectSelected(id);
        setCameraMode(FOLLOW_PLANET);
    }

    return id != -1;
}

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets) {
    displayInfoView(imageTextures);
    
    switch (getWindowType()) {
        case PLANET_WINDOW:
            displayPlanetWindow(imageTextures, textTextures, planets);
            break;
            
        case BASIC_SHIP_WINDOW:
            displayBasicShipWindow(imageTextures, textTextures, ships);
            break;
            
        case SHIP_WINDOW:
            displayShipWindow(imageTextures, textTextures, ships, planets);
            break;

        case SIDE_BAR_WINDOW:
            displaySideBar();
            break;

        case PAUSE_WINDOW:
            displayPauseWindow(imageTextures, textTextures);
            break;
            
        default:
            break;
    }
}

void displaySelectionCircle(void) {
    if (selectionMode == 1) {
        SDL_Point currentMouse = getMouseCoordinates();
        float radius = distancePointPoint(&centerSelectionCircle, &currentMouse);

        // Eviter que le cercle soit trop grand
        if (radius > SCREEN_WIDTH / 4) {  
            radius = SCREEN_WIDTH / 4;
        }
        
        drawCircle(WHITE, centerSelectionCircle.x, centerSelectionCircle.y, radius);
    }
}

void objetInSelectionCircle(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    SDL_Point currentMouse = getMouseCoordinates();  // Dans le referentiel de l'ecran
    SDL_Point object;

    for (int i = 0; i < shipCount; i++) {
        object = (SDL_Point){(ships[i].x + ships[i].w / 2 - getCameraRect().x - SCREEN_WIDTH / 2.0f) * getCameraScale() + SCREEN_WIDTH / 2.0f,
                             (ships[i].y + ships[i].h / 2 - getCameraRect().y - SCREEN_HEIGHT / 2.0f) * getCameraScale() + SCREEN_HEIGHT / 2.0f
                            };
        if (distancePointPoint(&object, &centerSelectionCircle) -  ships[i].w / 2
            < distancePointPoint(&currentMouse, &centerSelectionCircle)) {
            Mix_PlayChannel(1, sounds[4], 0);
            setWindowType(BASIC_SHIP_WINDOW);
            setWindowId(i);
            initBasicShipWindow(textTextures, fonts, &(ships[i]), ships, planets);        
            setCameraLastObjectSelected(i);
            setCameraMode(FOLLOW_SHIP);
            updateCameraFollow(ships, NULL);
            return;
        }
    }

    for (int i = 0; i < planetCount; i++) {
        object = (SDL_Point){(planets[i].x - getCameraRect().x - SCREEN_WIDTH / 2.0f) * getCameraScale() + SCREEN_WIDTH / 2.0f,
                             (planets[i].y - getCameraRect().y - SCREEN_HEIGHT / 2.0f) * getCameraScale() + SCREEN_HEIGHT / 2.0f
                            };
        if (distancePointPoint(&object, &centerSelectionCircle) - planets[i].radius * getCameraScale()
            < distancePointPoint(&currentMouse, &centerSelectionCircle)) {
            Mix_PlayChannel(1, sounds[3], 0);
            setWindowType(PLANET_WINDOW);
            setWindowId(i);
            initPlanetWindow(textTextures, fonts, planets);
            return;
        }
    }
}

void updateGame(Ship **ships, int *shipCount, Planet *planets, int planetCount, Mix_Chunk **sounds) {
    updateShips(*ships, planets, *shipCount);
    updatePlanets(planets, *ships, *shipCount, planetCount);
    updateBuilds(planets, planetCount);
    updateCameraFollow(*ships, planets);
    updateAsteroids(planets,planetCount);
    updateTotalOre(planets, planetCount);
    updateTiles(*ships, *shipCount);
    updateEnemies(ships, shipCount);    
    updateLasers(ships, shipCount, sounds);
}
