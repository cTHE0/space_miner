#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "renderer.h"
#include "ship.h"
#include "window.h"
#include "ship_window.h"
#include "config.h"
#include "camera.h"
#include "event.h"
#include "tools.h"
#include "assets_gestion.h"
#include "text.h"


static BasicShipWindowButton buttonSelected = NO_BUTTON; // Lequel des 3 boutons est selectionne ? Au debut, aucun des boutons n'est selectionne

static SDL_Rect bgRect,
                shipPictureRect,
                leftArrowRect, 
                rightArrowRect,
                line1Rect,
                crossRect,
                button1Rect,
                button2Rect,
                button3Rect,
                windowTitleRect,
                selectNewBaseRect,
                selectNewTargetRect;


void initBasicShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Importe la texture decrivant le titre de la page d'info. de la fusee
    sprintf(newText.text, "Ship #%d – Basic Rocket", getWindowId() + 1);  // Construction "de Ship #1,887 – Basic Rocket"
    newText.color = BLACK;
    newText.font = fonts[0];

    updateTextTexture(&textTextures[33], newText);

    SDL_QueryTexture(textTextures[33], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = SCREEN_WIDTH * 0.28;
    windowTitleRect.y = SCREEN_HEIGHT * 0.753;
    windowTitleRect.w = textureWidth * SCREEN_HEIGHT * 0.0004;
    windowTitleRect.h = textureHeight * SCREEN_HEIGHT * 0.0004;
}

void initBasicShipWindowRects(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;

    bgRect.x = SCREEN_WIDTH * 0.1; 
    bgRect.y = SCREEN_HEIGHT * 0.75; 
    bgRect.w = SCREEN_WIDTH * 0.8; 
    bgRect.h = SCREEN_HEIGHT * 0.25; 

    shipPictureRect.x = SCREEN_WIDTH * 0.13; 
    shipPictureRect.y = SCREEN_HEIGHT * 0.75; 
    shipPictureRect.w = SCREEN_HEIGHT * 0.25; 
    shipPictureRect.h = SCREEN_HEIGHT * 0.25; 

    leftArrowRect.x = SCREEN_WIDTH * 0.105; 
    leftArrowRect.y = SCREEN_HEIGHT * 0.86; 
    leftArrowRect.w = SCREEN_HEIGHT * 0.031; 
    leftArrowRect.h = SCREEN_HEIGHT * 0.031; 

    rightArrowRect.x = SCREEN_WIDTH * 0.88; 
    rightArrowRect.y = SCREEN_HEIGHT * 0.86; 
    rightArrowRect.w = SCREEN_HEIGHT * 0.031; 
    rightArrowRect.h = SCREEN_HEIGHT * 0.031; 

    line1Rect.x = SCREEN_WIDTH * 0.272; 
    line1Rect.y = SCREEN_HEIGHT * 0.78; 
    line1Rect.w = SCREEN_WIDTH * 0.627; 
    line1Rect.h = 3; 

    crossRect.x = SCREEN_WIDTH * 0.885; 
    crossRect.y = SCREEN_HEIGHT * 0.757; 
    crossRect.w = SCREEN_HEIGHT * 0.02; 
    crossRect.h = SCREEN_HEIGHT * 0.02; 

    button1Rect.x = SCREEN_WIDTH * 0.43; 
    button1Rect.y = SCREEN_HEIGHT * 0.69; 
    button1Rect.w = SCREEN_HEIGHT * 0.05; 
    button1Rect.h = SCREEN_HEIGHT * 0.05; 

    button2Rect.x = SCREEN_WIDTH * 0.49; 
    button2Rect.y = SCREEN_HEIGHT * 0.69; 
    button2Rect.w = SCREEN_HEIGHT * 0.05; 
    button2Rect.h = SCREEN_HEIGHT * 0.05; 

    button3Rect.x = SCREEN_WIDTH * 0.55; 
    button3Rect.y = SCREEN_HEIGHT * 0.69; 
    button3Rect.w = SCREEN_HEIGHT * 0.05; 
    button3Rect.h = SCREEN_HEIGHT * 0.05;

    SDL_QueryTexture(textTextures[45], NULL, NULL, &textureWidth, &textureHeight);
    selectNewBaseRect.x = SCREEN_WIDTH * 0.465;
    selectNewBaseRect.y = SCREEN_HEIGHT * 0.753;
    selectNewBaseRect.w = textureWidth * SCREEN_HEIGHT * 0.0004;
    selectNewBaseRect.h = textureHeight * SCREEN_HEIGHT * 0.0004;

    SDL_QueryTexture(textTextures[46], NULL, NULL, &textureWidth, &textureHeight);
    selectNewTargetRect.x = SCREEN_WIDTH * 0.465;
    selectNewTargetRect.y = SCREEN_HEIGHT * 0.753;
    selectNewTargetRect.w = textureWidth * SCREEN_HEIGHT * 0.0004;
    selectNewTargetRect.h = textureHeight * SCREEN_HEIGHT * 0.0004;
}

void displayBasicShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships){
    /*
    // Afficher la portee de la fusee
    SDL_Rect rangeCircle = (SDL_Rect){ships[getWindowId()].destRect.x - getCameraScale() * ships[getWindowId()].range, 
                             ships[getWindowId()].destRect.y - getCameraScale() * ships[getWindowId()].range, 
                             2 * getCameraScale() * ships[getWindowId()].range, 
                             2 * getCameraScale() * ships[getWindowId()].range};
    SDL_RenderCopy(renderer, imageTextures[5][4], NULL, &rangeCircle);
    */

    // Afficher fenetre du bas
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);                                       //Afficher fond
    SDL_RenderCopy(renderer, imageTextures[8][0], NULL, &shipPictureRect);                              //Afficher "photo" du ship
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &leftArrowRect);                                //Afficher fleche gauche
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &rightArrowRect, 180, NULL, SDL_FLIP_NONE);   // Afficher fleche droite
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &line1Rect);                                                           // Ligne horizontale sous nom fusee

    // Affichage boutons au-dessus fenetre
    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button1Rect);
    SDL_RenderCopy(renderer, imageTextures[2][4], NULL, &button1Rect);

    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button2Rect);
    SDL_RenderCopy(renderer, imageTextures[2][5], NULL, &button2Rect);

    SDL_RenderCopy(renderer, imageTextures[2][3], NULL, &button3Rect);
    SDL_RenderCopy(renderer, imageTextures[2][6], NULL, &button3Rect);

    // Afficher la ligne en pointillees 
    if (buttonSelected == BASE_BUTTON || buttonSelected == TARGET_BUTTON) {
        SDL_Point centerShipCoord = {ships[getWindowId()].destRect.x + ships[getWindowId()].destRect.w / 2,
                                     ships[getWindowId()].destRect.y + ships[getWindowId()].destRect.h / 2};
        plotPath(centerShipCoord, getMouseCoordinates(), 10, 5, WHITE);
    }

    // Afficher le nom de la fusee
    SDL_RenderCopy(renderer, textTextures[33], NULL, &windowTitleRect);

    // Affiche 'select a new base/target' quand necessaire
    if (buttonSelected == BASE_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[45], NULL, &selectNewBaseRect);
    } else if (buttonSelected == TARGET_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[46], NULL, &selectNewTargetRect);
    }
}

void basicShipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse) {
    // Changement du bouton selectionne dans la fenetre basique d'information
    if (SDL_PointInRect(&mouse, &button1Rect)) {
        setWindowType(SHIP_WINDOW);
        buttonSelected = NO_BUTTON;
        initShipWindow(textTextures, fonts, &ships[getWindowId()]);
    } else if (SDL_PointInRect(&mouse, &button2Rect)) {
        buttonSelected = BASE_BUTTON;
    } else if (SDL_PointInRect(&mouse, &button3Rect)) {
        buttonSelected = ATTACK_BUTTON;
    } 

    // Changement de la fusee observee
    else if (SDL_PointInRect(&mouse, &rightArrowRect)) {
        setWindowId((getWindowId() + 1) % shipCount);
        initBasicShipWindow(textTextures, fonts, &ships[getWindowId()]);
        setCenterCamera((SDL_Point){ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2});
        setCameraLastObjectSelected(getWindowId());
        updateCameraFollow(ships, NULL);
    }
    else if (SDL_PointInRect(&mouse, &leftArrowRect)) {
        if (getWindowId() == 0) {
            setWindowId(shipCount - 1);
        } else {
            setWindowId(getWindowId() - 1);
        }
        initBasicShipWindow(textTextures, fonts, &ships[getWindowId()]);
        setCenterCamera((SDL_Point){ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2});
        setCameraLastObjectSelected(getWindowId());
        updateCameraFollow(ships, NULL);
    }

    // Aucun des boutons de la fenetre n'a ete clique :
    else {
        // Gestion des actions en fonction du bouton appuye
        switch (buttonSelected) {
            case BASE_BUTTON:  // Equivaut a faire if (BASE_BUTTON || TARGET_BUTTON) {...}
            case TARGET_BUTTON:
                choosingNewBaseOrTarget(ships, shipCount, planets, planetCount, mouse);
                break;

            case NO_BUTTON:
                if (!clickOnShip(textTextures, fonts, ships, shipCount, mouse) && 
                    !clickOnPlanet(textTextures, fonts, planets, planetCount, mouse)) {
                    if (SDL_PointInRect(&mouse, &crossRect) || !clickOnBasicShipWindow(mouse)) {
                        setWindowType(NO_WINDOW);
                    }
                } else {  // Initialisation de buttonSelected si l'on clic sur une fusee/planet
                    buttonSelected = NO_BUTTON;
                }
                break;

            default:
                break;
        }
    }
}

void choosingNewBaseOrTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse) {
    int planetChosen = whichPlanetIsClicked(planets, planetCount, mouse);
    int shipChosen = whichShipIsClicked(ships, shipCount, mouse);
    Spot *spotDest = ((buttonSelected == BASE_BUTTON) ? &ships[getWindowId()].base : &ships[getWindowId()].target);

    if (planetChosen != -1) {  // La nouvelle cible est une planete
        spotDest->type = SPOT_PLANET;
        spotDest->planet = &planets[planetChosen];
    } else if (shipChosen != -1) {  // La nouvelle cible est un ship (par ex. une station spatiale, orbitale ou un vaisseau de ravitaillement)
        spotDest->type = SPOT_SHIP;
        spotDest->ship = &ships[shipChosen];
    } else {  // La nouvelle target est un point random de l'espace
        spotDest->type = SPOT_POINT;
        spotDest->point = (SDL_Point){(mouse.x - SCREEN_WIDTH / 2.f) / getCameraScale() + getCameraRect().x + SCREEN_WIDTH / 2.f,
                                      (mouse.y - SCREEN_HEIGHT / 2.f) / getCameraScale() + getCameraRect().y + SCREEN_HEIGHT / 2.f};
    }

    // Si la fusee attend sans rien faire, redemarrage
    Ship *ship = &ships[getWindowId()];
    if ((ship->state == WAITING_ON_BASE && ship->base.type != SPOT_PLANET) ||
        (ship->state == WAITING_ON_TARGET && ship->target.type != SPOT_PLANET)) {
        ship->state = (ship->state == WAITING_ON_BASE) ? MOVING_TO_TARGET : MOVING_TO_BASE;
    }

    // Le choix a ete fait, actualisation de l'etat du bouton
    buttonSelected = (buttonSelected == BASE_BUTTON) ? TARGET_BUTTON : NO_BUTTON;
}

int clickOnBasicShipWindow(SDL_Point mouse) {
    return SDL_PointInRect(&mouse, &bgRect);
}

void setButtonSelected(BasicShipWindowButton newButton) {
    buttonSelected = newButton;
}

BasicShipWindowButton getButtonSelected(void) {
    return buttonSelected;
}