#include "window.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"


int whichWindowShip = -1;

// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static SDL_Rect srcRectShip = {0, 0, 64, 64};
static SDL_Rect targetDisplayedRect;
static SDL_Rect baseDisplayedRect;
static SDL_Rect destRectShip;
static SDL_Rect narrowRect;
static SDL_Rect windowTitleRect;
static SDL_Rect windowLine1Rect;
static SDL_Rect windowLine2Rect;
static SDL_Rect windowLine3Rect;
static SDL_Rect windowLine4Rect;
static SDL_Rect category1TitleRect;
static SDL_Rect category2TitleRect;
static SDL_Rect category3TitleRect;
static SDL_Rect category4TitleRect;
static SDL_Rect category5TitleRect;

void initRectShipWindow(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes

    baseDisplayedRect.x = windowRect.x + windowRect.w * 0.05;
    baseDisplayedRect.y = windowRect.y + windowRect.h * 0.18;
    baseDisplayedRect.w = windowRect.w * 0.12;
    baseDisplayedRect.h = windowRect.w * 0.12;

    targetDisplayedRect.x = windowRect.x + windowRect.w * 0.38;
    targetDisplayedRect.y = windowRect.y + windowRect.h * 0.18;
    targetDisplayedRect.w = windowRect.w * 0.12;
    targetDisplayedRect.h = windowRect.w * 0.12;

    destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    destRectShip.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.13;
    destRectShip.w = windowRect.w * 0.1;
    destRectShip.h = windowRect.w * 0.1;

    narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    narrowRect.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.5;
    narrowRect.w = 4;
    narrowRect.h = 15;

    SDL_QueryTexture(textTextures[4], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = windowRect.x + windowRect.w * 0.01;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;

    windowLine1Rect.x = windowRect.x;  // Grande barre horizontale haut
    windowLine1Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine1Rect.w = windowRect.w;
    windowLine1Rect.h = 3;

    windowLine2Rect.x = windowRect.x;  // Barre horizontale gauche milieu
    windowLine2Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine2Rect.w = windowRect.w * 0.55;
    windowLine2Rect.h = 3;

    windowLine3Rect.x = windowRect.x + windowRect.w * 0.55;  // Barre horizontale droite milieu
    windowLine3Rect.y = windowRect.y + windowRect.h * 0.6;
    windowLine3Rect.w = windowRect.w * 0.45;
    windowLine3Rect.h = 3;

    windowLine4Rect.x = windowRect.x + windowRect.w * 0.55;  // Barre verticale du milieu
    windowLine4Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine4Rect.w = 3;
    windowLine4Rect.h = windowRect.h * 0.93;

    SDL_QueryTexture(textTextures[5], NULL, NULL, &textureWidth, &textureHeight);
    category1TitleRect.x = windowRect.x + windowRect.w * 0.04;  // Travel information
    category1TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category1TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category1TitleRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[8], NULL, NULL, &textureWidth, &textureHeight);
    category2TitleRect.x = windowRect.x + windowRect.w * 0.04;  // Tank manager
    category2TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category2TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category2TitleRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[11], NULL, NULL, &textureWidth, &textureHeight);
    category3TitleRect.x = windowRect.x + windowRect.w * 0.58;  // Ship condition
    category3TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category3TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category3TitleRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[13], NULL, NULL, &textureWidth, &textureHeight);
    category4TitleRect.x = windowRect.x + windowRect.w * 0.58;  // Reported problems
    category4TitleRect.y = windowRect.y + windowRect.h * 0.32;
    category4TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category4TitleRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[14], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.58;  // Tank composition
    category5TitleRect.y = windowRect.y + windowRect.h * 0.61;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;
}

void displayShipWindow(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    initRectShipWindow(textTextures);

    if (windowOpened != SHIP_WINDOW) {  // La fenetre d'information d'une fusee est-elle ouverte ?
        return;
    }

    ShipWindowFondations(renderer, imageTextures, textTextures);
    ShipWindowTravelInfo(renderer, imageTextures, textTextures, ships);
    ShipWindowTravelTankManager(renderer, imageTextures, textTextures, ships);
    ShipWindowTravelShipCond(renderer, imageTextures, textTextures, ships);
    ShipWindowTravelTankCompo(renderer, imageTextures, textTextures, ships);
}

void ShipWindowFondations(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du bg
    SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &windowRect);

    // Affichage de la croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);

    // Affichage du titre de la page
    SDL_RenderCopy(renderer, textTextures[4], NULL, &windowTitleRect);

    // Affichages des barres qui separent les informations de la page
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &windowLine1Rect);  // Grande barre horizontale haut
    SDL_RenderFillRect(renderer, &windowLine2Rect);  // Barre horizontale milieu
    SDL_RenderFillRect(renderer, &windowLine3Rect);  // Barre horizontale droite milieu
    SDL_RenderFillRect(renderer, &windowLine4Rect);  // Barre verticale du milieu
}

void ShipWindowTravelInfo(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Travel information"
    SDL_RenderCopy(renderer, textTextures[5], NULL, &category1TitleRect);

    // Affichage des deux planetes
    SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].target->idPicture], NULL, &targetDisplayedRect);
    SDL_RenderCopy(renderer, imageTextures[5][ships[whichWindowShip].base->idPicture], NULL, &baseDisplayedRect);

    // Affichage du systeme de progression de la fusee dans l'espace
    int dp = targetDisplayedRect.x - baseDisplayedRect.x - baseDisplayedRect.w;  // Distance en pixel entre 2 planètes sur fenêtre
    float f;

    if (ships[whichWindowShip].state == MOVING_TO_TARGET || ships[whichWindowShip].state == WAITING_ON_BASE) {  
        // Fraction du chemin parcourue
        f = distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base)
            / (distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base) - ships[whichWindowShip].base->radius - ships[whichWindowShip].target->radius);  

        // Tracer la fleche
        narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        while (narrowRect.x < targetDisplayedRect.x) { 
            if (narrowRect.x >= (int)(baseDisplayedRect.x + baseDisplayedRect.w + f * dp)) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            SDL_RenderFillRect(renderer, &narrowRect);
            narrowRect.x += narrowRect.w * 2;
        }

        // Tracer la fusee
        destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w + f * dp - baseDisplayedRect.w * 0.3;
        SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 90, NULL, SDL_FLIP_NONE);

    } else if (ships[whichWindowShip].state == MOVING_TO_BASE || ships[whichWindowShip].state == WAITING_ON_TARGET) { 
        // Fraction du chemin parcourue
        f = distanceShipPlanet(ships[whichWindowShip], *ships[whichWindowShip].base)
            / (distancePlanetPlanet(*ships[whichWindowShip].target, *ships[whichWindowShip].base) - ships[whichWindowShip].base->radius - ships[whichWindowShip].target->radius); 

        // Tracer la fleche
        narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        while (narrowRect.x < targetDisplayedRect.x) {
            if (narrowRect.x >= (int)(baseDisplayedRect.x + baseDisplayedRect.w + f * dp)) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            } 
            SDL_RenderFillRect(renderer, &narrowRect);
            narrowRect.x += narrowRect.w * 2;
        }

        // Tracer la fusee
        destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w + f * dp - baseDisplayedRect.w * 0.53;
        SDL_RenderCopyEx(renderer, imageTextures[6][ships[whichWindowShip].idPicture], &srcRectShip, &destRectShip, 270, NULL, SDL_FLIP_NONE);
    }
}

void ShipWindowTravelTankManager(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Tank manager"
    SDL_RenderCopy(renderer, textTextures[8], NULL, &category2TitleRect);
}

void ShipWindowTravelShipCond(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Ship condition"
    SDL_RenderCopy(renderer, textTextures[11], NULL, &category3TitleRect);

    // Affichage du titre "Reported problemes"
    SDL_RenderCopy(renderer, textTextures[13], NULL, &category4TitleRect);
}

void ShipWindowTravelTankCompo(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Tank composition"
    SDL_RenderCopy(renderer, textTextures[14], NULL, &category5TitleRect);
}
