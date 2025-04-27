#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "camera.h"
#include "basic_ship_window.h"


static Window windowInfo = {NO_WINDOW, 0};

static const SDL_Rect windowRect = {(1 - 0.8) * SCREEN_WIDTH / 2., 
                       (1 - 0.8) * SCREEN_HEIGHT / 2., 
                       SCREEN_WIDTH * 0.8,
                       SCREEN_HEIGHT * 0.8};
                       
static const SDL_Rect WindowCrossRect = {(1 + 0.8) / 2. * SCREEN_WIDTH - (SCREEN_WIDTH * 0.8) * 0.03, 
                            (1 - 0.8) * SCREEN_HEIGHT / 2 + 20,
                            (SCREEN_WIDTH * 0.8) * 0.015,
                            (SCREEN_WIDTH * 0.8) * 0.015};

static const SDL_Rect basicShipWindowCrossRect = {
                                                    11*SCREEN_WIDTH/12 - 3*SCREEN_HEIGHT/128, 
                                                    3*SCREEN_HEIGHT/4 + SCREEN_HEIGHT/128, 
                                                    SCREEN_HEIGHT/64, 
                                                    SCREEN_HEIGHT/64
                                                };


int clickOnWindow(SDL_Point mouse) {
    return SDL_PointInRect(&mouse, &windowRect);
}

WindowType getWindowType(void) {
    return windowInfo.type;
}

int getWindowId(void) {
    return windowInfo.id;
}

void changeWindowType(WindowType newType) {
    windowInfo.type = newType;
}

void changeWindowId(int newId) {
    windowInfo.id = newId;
}

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    switch (getWindowType()) {

        case NO_WINDOW:
            clickOnShip(textTextures, fonts, ships, shipCount, mouse);
            clickOnPlanet(textTextures, fonts, planets, planetCount, mouse);
            break;

        case SHIP_WINDOW:  // Ce cas est fusionne avec le prochain (<=> (SHIP_WINDOW || PLANET_WINDOW) )

        case PLANET_WINDOW:
            if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
                changeWindowType(NO_WINDOW);
            }
            break;

        case BASIC_SHIP_WINDOW:
            if (SDL_PointInRect(&mouse, &basicShipWindowCrossRect)) {
                changeWindowType(NO_WINDOW);
            }
            // Si aucun des bouton au-dessus fenêtre n'a était touché alors on peu choisir nouvelle target 
            // (évite bug en sélectionnant planète, ship ou point sous le bouton en question)
            if (!changeButtonType(mouse)) {
                choosingNewTarget(ships, shipCount, planets, planetCount, mouse);
            };      
            break;

        default:
            break;
    }
}

int whichShipIsClicked(Ship *ships, int shipCount, SDL_Point mouse){
    // Renvoie l'id du ship selectionne ou -1 sinon
    for (int i = 0; i < shipCount; i++) {
        if (SDL_PointInRect(&mouse, &ships[i].destRect)) {
            return i;
        }
    }
    return -1;
}

void clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, SDL_Point mouse) {
    int id = whichShipIsClicked(ships, shipCount, mouse); // // id du ship selectionne ou -1 sinon

    if (id != -1){ // Si un ship a etait clique...
        changeWindowId(id);
        changeWindowType(BASIC_SHIP_WINDOW); 
        changeWindowId(id);
        //initShipWindow(textTextures, fonts, ships);//Ne s'ouvrira que ds un 2ème temps selon action joueur
    }
}

int whichPlanetIsClicked(Planet *planets, int planetCount, SDL_Point mouse){
    // Renvoie la planète sur lequelle user a cliqué ou -1 sinon
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

void clickOnPlanet(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, int planetCount, SDL_Point mouse) {
    int i = whichPlanetIsClicked(planets, planetCount, mouse);
    if (i != -1){
        changeWindowId(i);
        changeWindowType(PLANET_WINDOW);
        initPlanetWindow(textTextures, fonts);
    }
}

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int planetCount) {
    displayShipWindow(imageTextures, textTextures, ships);
    displayPlanetWindow(imageTextures, textTextures, planets, planetCount);
    displayBasicShipWindow(imageTextures, ships);
}
