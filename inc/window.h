#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"

typedef enum {NO_WINDOW, SHIP_WINDOW, PLANET_WINDOW, DEFENDER_WINDOW, ENNEMY_WINDOW, BASIC_SHIP_WINDOW} WindowType;

typedef struct {
    WindowType type;  // Type de fenetre afiche (d'une fusee, planete, attaquant, etc)
    uint32_t id;      // Stocke l'id de l'objet (fusee, planete, etc) affiche sur la fenetre
} Window;

void displayWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int planetCount);
        void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
                void initShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships);
                        void initRectShipWindow(SDL_Texture **textTextures);
                        void initTextShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships);
                void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
                void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
                void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
                void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships);
                void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
        void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount);
                void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts);
                    void initRectPlanetWindow(SDL_Texture **textTextures);
                    void initTextPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts);

void openWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount);
    void clickOnShip(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, SDL_Point mouse);
    void clickOnPlanet(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets, int planetCount, SDL_Point mouse);
    int clickOnWindow(SDL_Point mouse);

WindowType getWindowType(void);
int getWindowId(void);
void changeWindowType(WindowType newType);
void changeWindowId(int newId);

#endif
