#include "window.h"

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "assets_gestion.h"
#include "renderer.h"
#include "tools.h"


// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static char baseName[64] = {0};
static char targetName[64] = {0};

static SDL_Rect srcRectShip = {0, 0, 64, 64},
                windowRect,
                WindowCrossRect,
                targetDisplayedRect,
                baseDisplayedRect,
                destRectShip,
                narrowRect,
                windowTitleRect,
                windowLine1Rect,
                windowLine2Rect,
                windowLine3Rect,
                windowLine4Rect,
                category1TitleRect,
                category2TitleRect,
                category3TitleRect,
                category4TitleRect,
                category5TitleRect,
                baseDisplayedRect2,
                targetDisplayedRect2,
                modifyingTankRect,
                shipConditionRect,
                shipConditionSrcRect,
                nameBaseDisplayedRect,
                nameTargetDisplayedRect,
                shipFirstCompartmentRect,
                shipFirstCompartmentLogoRect;

void initShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships){
    initTextShipWindow(textTextures, fonts, ships);
    initRectShipWindow(textTextures);
}

void initTextShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships) {
    TextToLoad newText;

    // Importe le nom de la base de la fusee
    generateRandomName(baseName, currentSeed, ships[getWindowType()].base->id);
    strcpy(newText.text, (const char*)baseName);
    newText.color = BLACK;
    newText.font = fonts[0];

    // Creation de la texture de la base de la fusee
    updateTextTexture(&textTextures[30], newText);

    // Importe le nom de la cible de la fusee
    generateRandomName(targetName, currentSeed, ships[getWindowType()].target.planet->id);
    strcpy(newText.text, (const char*)targetName);
    newText.color = BLACK;
    newText.font = fonts[0];

    // Creation de la texture de la cible de la fusee
    updateTextTexture(&textTextures[31], newText);
}

void initRectShipWindow(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes


    // ShipWindowFondations(imageTextures, textTextures);

    windowRect.x = (1 - 0.8) * SCREEN_WIDTH / 2.;
    windowRect.y = (1 - 0.8) * SCREEN_HEIGHT / 2.;
    windowRect.w = SCREEN_WIDTH * 0.8;
    windowRect.h = SCREEN_HEIGHT * 0.8;

    WindowCrossRect.x = windowRect.x + windowRect.w * 0.97;
    WindowCrossRect.y = windowRect.y + windowRect.h * 0.025;
    WindowCrossRect.w = windowRect.w * 0.015;
    WindowCrossRect.h = windowRect.w * 0.015;

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


    // ShipWindowTravelInfo(imageTextures, textTextures, ships);

    SDL_QueryTexture(textTextures[5], NULL, NULL, &textureWidth, &textureHeight);
    category1TitleRect.x = windowRect.x + windowRect.w * 0.04;  // Travel information
    category1TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category1TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category1TitleRect.h = textureHeight * windowRect.w * 0.0005;

    baseDisplayedRect.x = windowRect.x + windowRect.w * 0.02;
    baseDisplayedRect.y = windowRect.y + windowRect.h * 0.18;
    baseDisplayedRect.w = windowRect.w * 0.12;
    baseDisplayedRect.h = windowRect.w * 0.12;

    targetDisplayedRect.x = windowRect.x + windowRect.w * 0.42;
    targetDisplayedRect.y = windowRect.y + windowRect.h * 0.18;
    targetDisplayedRect.w = windowRect.w * 0.12;
    targetDisplayedRect.h = windowRect.w * 0.12;

    SDL_QueryTexture(textTextures[30], NULL, NULL, &textureWidth, &textureHeight);
    nameBaseDisplayedRect.x = windowRect.x + windowRect.w * 0.03;
    nameBaseDisplayedRect.y = windowRect.y + windowRect.h * 0.4;
    nameBaseDisplayedRect.w = textureWidth * windowRect.w * 0.0005;
    nameBaseDisplayedRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[31], NULL, NULL, &textureWidth, &textureHeight);
    nameTargetDisplayedRect.x = windowRect.x + windowRect.w * 0.38;
    nameTargetDisplayedRect.y = windowRect.y + windowRect.h * 0.4;
    nameTargetDisplayedRect.w = textureWidth * windowRect.w * 0.0005;
    nameTargetDisplayedRect.h = textureHeight * windowRect.w * 0.0005;

    destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    destRectShip.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.13;
    destRectShip.w = windowRect.w * 0.1;
    destRectShip.h = windowRect.w * 0.1;

    narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    narrowRect.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.5;
    narrowRect.w = 4;
    narrowRect.h = 15;


    // ShipWindowTankManager(imageTextures, textTextures, ships);

    SDL_QueryTexture(textTextures[8], NULL, NULL, &textureWidth, &textureHeight);
    category2TitleRect.x = windowRect.x + windowRect.w * 0.04;  // Tank manager
    category2TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category2TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category2TitleRect.h = textureHeight * windowRect.w * 0.0005;

    baseDisplayedRect2.x = windowRect.x + windowRect.w * 0.02;
    baseDisplayedRect2.y = windowRect.y + windowRect.h * 0.7;
    baseDisplayedRect2.w = windowRect.w * 0.09;
    baseDisplayedRect2.h = windowRect.w * 0.09;

    targetDisplayedRect2.x = windowRect.x + windowRect.w * 0.45;
    targetDisplayedRect2.y = windowRect.y + windowRect.h * 0.7;
    targetDisplayedRect2.w = windowRect.w * 0.09;
    targetDisplayedRect2.h = windowRect.w * 0.09;

    SDL_QueryTexture(textTextures[15], NULL, NULL, &textureWidth, &textureHeight);
    modifyingTankRect.x = windowRect.x + windowRect.w * 0.25;
    modifyingTankRect.y = windowRect.y + windowRect.h * 0.59;
    modifyingTankRect.w = textureWidth * windowRect.w * 0.0005;
    modifyingTankRect.h = textureHeight * windowRect.w * 0.0005;


    // ShipWindowShipCond(imageTextures, textTextures, ships);

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

    shipConditionRect.x = windowRect.x + windowRect.w * 0.58;
    shipConditionRect.y = windowRect.y + windowRect.h * 0.12;
    shipConditionRect.w = windowRect.w * 0.13;
    shipConditionRect.h = windowRect.w * 0.13;

    shipConditionSrcRect.x = 0;
    shipConditionSrcRect.y = 0;
    shipConditionSrcRect.w = 64;
    shipConditionSrcRect.h = 64;


    // ShipWindowTankCompo(imageTextures, textTextures);   
    SDL_QueryTexture(textTextures[14], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.58;  // Tank composition
    category5TitleRect.y = windowRect.y + windowRect.h * 0.61;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;

    shipFirstCompartmentRect.x = windowRect.x + windowRect.w * 0.85;
    shipFirstCompartmentRect.y = windowRect.y + windowRect.h * 0.88;
    shipFirstCompartmentRect.w = windowRect.w * 0.07;
    shipFirstCompartmentRect.h = windowRect.h * 0.07;

    shipFirstCompartmentLogoRect.x = windowRect.x + windowRect.w * 0.93;
    shipFirstCompartmentLogoRect.y = windowRect.y + windowRect.h * 0.89;
    shipFirstCompartmentLogoRect.w = windowRect.w * 0.04;
    shipFirstCompartmentLogoRect.h = windowRect.w * 0.04;
}

void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    
    if (getWindowType() != SHIP_WINDOW) {  // La fenetre d'information d'une fusee est-elle ouverte ?
        return;
    }

    ShipWindowFondations(imageTextures, textTextures);
    ShipWindowTravelInfo(imageTextures, textTextures, ships);
    ShipWindowTankManager(imageTextures, textTextures, ships);
    ShipWindowShipCond(imageTextures, textTextures, ships);
    ShipWindowTankCompo(imageTextures, textTextures, ships);
}

void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du bg
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &windowRect);

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

void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Travel information"
    SDL_RenderCopy(renderer, textTextures[5], NULL, &category1TitleRect);

    // Affichage des deux planetes
    int idPicture;
    idPicture = (ships[getWindowId()].base->planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].base->id) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &baseDisplayedRect);
    idPicture = (ships[getWindowId()].target.planet->planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].target.planet->id) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &targetDisplayedRect);

    // Affichage du nom des deux planetes
    SDL_RenderCopy(renderer, textTextures[30], NULL, &nameBaseDisplayedRect);
    SDL_RenderCopy(renderer, textTextures[31], NULL, &nameTargetDisplayedRect);

    // Affichage du systeme de progression de la fusee dans l'espace
    int dp = targetDisplayedRect.x - baseDisplayedRect.x - baseDisplayedRect.w;  // Distance en pixel entre 2 planètes sur fenêtre
    float f;

    if (ships[getWindowId()].state == MOVING_TO_TARGET || ships[getWindowId()].state == WAITING_ON_BASE) {  
        // Fraction du chemin parcourue
        f = distanceShipPlanet(ships[getWindowId()], *ships[getWindowId()].base)
            / (distancePlanetPlanet(*ships[getWindowId()].target.planet, *ships[getWindowId()].base) - ships[getWindowId()].base->radius - ships[getWindowId()].target.planet->radius);

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
        SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &srcRectShip, &destRectShip, 90, NULL, SDL_FLIP_NONE);

    } else if (ships[getWindowId()].state == MOVING_TO_BASE || ships[getWindowId()].state == WAITING_ON_TARGET) { 
        // Fraction du chemin parcourue
        f = distanceShipPlanet(ships[getWindowId()], *ships[getWindowId()].base)
            / (distancePlanetPlanet(*ships[getWindowId()].target.planet, *ships[getWindowId()].base) - ships[getWindowId()].base->radius - ships[getWindowId()].target.planet->radius);

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
        SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &srcRectShip, &destRectShip, 270, NULL, SDL_FLIP_NONE);
    }
}

void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Tank manager"
    SDL_RenderCopy(renderer, textTextures[8], NULL, &category2TitleRect);

    // Affichage des deux planetes
    int idPicture;
    idPicture = (ships[getWindowId()].base->planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].base->id) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &baseDisplayedRect2);
    idPicture = (ships[getWindowId()].target.planet->planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].target.planet->id) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &targetDisplayedRect2);

    // Affichage du tank en cours de modification ("Tank 1")
    SDL_RenderCopy(renderer, textTextures[15], NULL, &modifyingTankRect);
}

void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Ship condition"
    SDL_RenderCopy(renderer, textTextures[11], NULL, &category3TitleRect);

    // Affichage de la fusee
    SDL_RenderCopy(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipConditionSrcRect, &shipConditionRect);

    // Affichage du titre "Reported problemes"
    SDL_RenderCopy(renderer, textTextures[13], NULL, &category4TitleRect);
}

void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Tank composition"
    SDL_RenderCopy(renderer, textTextures[14], NULL, &category5TitleRect);

    // Affichage des reservoirs, l'un apres l'autre
    Cargo cargo = ships[getWindowId()].cargo;
    SDL_Rect currentTankRect = shipFirstCompartmentRect;
    SDL_Rect currentLogoRect = shipFirstCompartmentLogoRect;
    for (int i = 0; i < cargo.compartmentsNumber; i++) {
        // Barre de fond
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        currentTankRect.w = shipFirstCompartmentRect.w;
        SDL_RenderFillRect(renderer, &currentTankRect);

        // Barre de niveau actuel
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        currentTankRect.w = shipFirstCompartmentRect.w * cargo.compartmentsList[i].currentCapacity / cargo.compartmentsList[i].maxCapacity;
        SDL_RenderFillRect(renderer, &currentTankRect);

        // Logo du type de minerai
        switch (cargo.compartmentsList[i].ore) {
            case FUEL:
                SDL_RenderCopy(renderer, imageTextures[4][0], NULL, &currentLogoRect);
                break;
            default:
                SDL_RenderCopy(renderer, imageTextures[4][1], NULL, &currentLogoRect);
                break;
        }

        // Pour afficher le prochain reservoir
        currentTankRect.y -= 1.05 * shipFirstCompartmentRect.h;
        currentLogoRect.y -= 1.05 * shipFirstCompartmentRect.h;
    }
}
