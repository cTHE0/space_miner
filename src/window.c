#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "camera.h"
#include "basic_ship_window.h"
#include "ship_window.h"
#include "planet_window.h"
#include "info_view.h"
#include "side_bar_window.h"


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

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    switch (getWindowType()) {

        case SIDE_BAR_WINDOW:
        case NO_WINDOW:
            if (clickSideBar(mouse)) break;
            if (clickOnShip(textTextures, fonts, ships, shipCount, mouse)) break;
            if (clickOnPlanet(textTextures, fonts, planets, planetCount, mouse)) break;
            break;

        case BASIC_SHIP_WINDOW:
            basicShipWindowGestion(textTextures, fonts, ships, shipCount, planets, planetCount, mouse);
            break;

        case SHIP_WINDOW:
            shipWindowGestion(textTextures, fonts, &ships[getWindowId()], planets, mouse);
            break;

        case PLANET_WINDOW:
            planetWindowGestion(textTextures, fonts, planets, mouse);
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

int clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, SDL_Point mouse) {
    int id = whichShipIsClicked(ships, shipCount, mouse); // id du ship selectionne ou -1 sinon

    if (id != -1) {  // Si un ship a etait clique...
        setWindowId(id);
        setWindowType(BASIC_SHIP_WINDOW); 
        initBasicShipWindow(textTextures, fonts, &ships[id]);
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
            displayShipWindow(imageTextures, textTextures, ships);
            break;

        case SIDE_BAR_WINDOW:
            displaySideBar();
            break;
            
        default:
            break;
    }
}
