#include "basic_ship_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
#include "place.h"


static BasicShipWindowButton buttonSelected = NO_BUTTON; // Lequel des 3 boutons est selectionne ? Au debut, aucun des boutons n'est selectionne

static SDL_Rect bgRect,
                shipPictureRect,
                leftArrowRect, 
                rightArrowRect,
                line1Rect,
                crossRect,
                button1Rect,
                button2Rect,
                windowTitleRect,
                selectNewBaseRect,
                selectNewTargetRect,
                shipConditionSrcRect,
                shipBgRect,
                mainInfoTanksEdgeRect,
                mainInfoTanksInfoRect,
                shipFirstCompartmentRect,
                shipFirstCompartmentLogoRect,
                shipFirstCompartmentNumberRect,
                logoBaseRect,
                logoTargetRect;

static SDL_Point centerBaseCoord,
                 centerTargetCoord;


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

    // Calcul des coordonnees de la base et de la cible dans le referentiel de la map
    updateNarrowBasicShipWindow(ship);
}

void updateNarrowBasicShipWindow(Ship *ship) {  // Calcul des coordonnees de la base et de la cible dans le referentiel de la map
    switch (ship->base.type) {
        case SPOT_PLANET:
            centerBaseCoord = (SDL_Point){ship->base.planet->x, ship->base.planet->y};
            break;
        case SPOT_SHIP:
            centerBaseCoord = (SDL_Point){ship->base.ship->x + ship->base.ship->w / 2, 
                                          ship->base.ship->y + ship->base.ship->h / 2};
            break;
        case SPOT_POINT:
            centerBaseCoord = ship->base.point;
            break;
        default:
            centerBaseCoord = (SDL_Point){1000, 1000};
            break;
    }
    switch (ship->target.type) {
        case SPOT_PLANET:
            centerTargetCoord = (SDL_Point){ship->target.planet->x, ship->target.planet->y};
            break;
        case SPOT_SHIP:
            centerTargetCoord = (SDL_Point){ship->target.ship->x + ship->target.ship->w / 2, 
                                            ship->target.ship->y + ship->target.ship->h / 2};
            break;
        case SPOT_POINT:
            centerTargetCoord = ship->target.point;
            break;
        default:
            centerTargetCoord = (SDL_Point){0, 0};
            break;
    }

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

    button1Rect.x = SCREEN_WIDTH * 0.455; 
    button1Rect.y = SCREEN_HEIGHT * 0.69; 
    button1Rect.w = SCREEN_WIDTH * 0.03; 
    button1Rect.h = SCREEN_WIDTH * 0.03; 

    button2Rect.x = SCREEN_WIDTH * 0.515; 
    button2Rect.y = SCREEN_HEIGHT * 0.69; 
    button2Rect.w = SCREEN_WIDTH * 0.03; 
    button2Rect.h = SCREEN_WIDTH * 0.03; 

    SDL_QueryTexture(textTextures[45], NULL, NULL, &textureWidth, &textureHeight);
    selectNewBaseRect.x = SCREEN_WIDTH * 0.458;
    selectNewBaseRect.y = SCREEN_HEIGHT * 0.65;
    selectNewBaseRect.w = textureWidth * SCREEN_HEIGHT * 0.0004;
    selectNewBaseRect.h = textureHeight * SCREEN_HEIGHT * 0.0004;

    SDL_QueryTexture(textTextures[46], NULL, NULL, &textureWidth, &textureHeight);
    selectNewTargetRect.x = SCREEN_WIDTH * 0.458;
    selectNewTargetRect.y = SCREEN_HEIGHT * 0.65;
    selectNewTargetRect.w = textureWidth * SCREEN_HEIGHT * 0.0004;
    selectNewTargetRect.h = textureHeight * SCREEN_HEIGHT * 0.0004;

    shipConditionSrcRect = (SDL_Rect){0, 0, 64, 64};
    shipBgRect = (SDL_Rect){SCREEN_WIDTH * 0.6330, SCREEN_HEIGHT * 0.700, SCREEN_WIDTH * 0.3500, SCREEN_WIDTH * 0.2700};
    mainInfoTanksEdgeRect = (SDL_Rect){SCREEN_WIDTH * 0.7460, SCREEN_HEIGHT * 0.6410, SCREEN_WIDTH * 0.1300, SCREEN_WIDTH * 0.1350};

    SDL_QueryTexture(textTextures[47], NULL, NULL, &textureWidth, &textureHeight);
    mainInfoTanksInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.62, SCREEN_HEIGHT * 0.84, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};

    shipFirstCompartmentRect = (SDL_Rect){SCREEN_WIDTH * 0.7890, SCREEN_HEIGHT * 0.7930, SCREEN_WIDTH * 0.0500, SCREEN_WIDTH * 0.0210};

    shipFirstCompartmentLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.76600, SCREEN_HEIGHT * 0.7940, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0200};

    shipFirstCompartmentNumberRect = (SDL_Rect){SCREEN_WIDTH * 0.8070, SCREEN_HEIGHT * 0.7870, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0280};

    logoBaseRect = (SDL_Rect){0, 0, SCREEN_WIDTH * 0.0700, SCREEN_WIDTH * 0.0700};
    logoTargetRect = (SDL_Rect){0, 0, SCREEN_WIDTH * 0.0700, SCREEN_WIDTH * 0.0700};
}

void displayBasicShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    basicShipWindowNarrowBaseTarget(imageTextures);
    basicShipWindowFondations(imageTextures, textTextures);
    basicShipWindowInfos(imageTextures, textTextures, ships);
}

void basicShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture ** textTextures) {
    // Afficher le fond de la fenetre
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);

    // Afficher le nom de la fusee
    SDL_RenderCopy(renderer, textTextures[33], NULL, &windowTitleRect);

    // Afficher l'image 'reelle' de la fusee
    SDL_RenderCopy(renderer, imageTextures[8][0], NULL, &shipPictureRect);

    // Afficher les fleches pour naviguer entre les fusees
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &leftArrowRect);
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &rightArrowRect, 180, NULL, SDL_FLIP_NONE);

    // Afficher la croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);

    // Afficher les lignes structurant la page
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &line1Rect);  

    // Afficher le fond des boutons au-dessus de la fenetre
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderFillRect(renderer, &button1Rect);
    SDL_RenderFillRect(renderer, &button2Rect);

    // Afficher les boutons au-dessus de la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][4], NULL, &button1Rect);
    SDL_RenderCopy(renderer, imageTextures[2][5], NULL, &button2Rect);
}

void basicShipWindowNarrowBaseTarget(SDL_Texture ***imageTextures) {
    // Affichage du trait reliant base et cible
    SDL_Point centerBaseCoordScreen = {(centerBaseCoord.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                       (centerBaseCoord.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};
    SDL_Point centerTargetCoordScreen = {(centerTargetCoord.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                         (centerTargetCoord.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};

    plotPath(centerBaseCoordScreen, centerTargetCoordScreen, 10, 5, WHITE);

    // Affichage du logo home
    logoBaseRect.w = 0.300 * SCREEN_WIDTH * getCameraScale();
    logoBaseRect.h = 0.300 * SCREEN_WIDTH * getCameraScale();
    logoBaseRect.x = centerBaseCoordScreen.x - logoBaseRect.w / 2;
    logoBaseRect.y = centerBaseCoordScreen.y - logoBaseRect.w / 2;
    SDL_RenderCopy(renderer, imageTextures[5][9], NULL, &logoBaseRect);

    // Affichage du logo target
    logoTargetRect.w = 0.300 * SCREEN_WIDTH * getCameraScale();
    logoTargetRect.h = 0.300 * SCREEN_WIDTH * getCameraScale();
    logoTargetRect.x = centerTargetCoordScreen.x - logoTargetRect.w / 2;
    logoTargetRect.y = centerTargetCoordScreen.y - logoTargetRect.w / 2;
    SDL_RenderCopy(renderer, imageTextures[5][8], NULL, &logoTargetRect);
}

void basicShipWindowInfos(SDL_Texture ***imageTextures, SDL_Texture ** textTextures, Ship *ships) {
    // Affiche la fusee en arriere plan (reservoir par dessus)
    SDL_RenderCopy(renderer, imageTextures[5][7], &shipConditionSrcRect, &shipBgRect);

    // Affiche le contour du schema du 'main info'
    SDL_RenderDrawRect(renderer, &mainInfoTanksEdgeRect);

    // Affiche le 'main info'
    SDL_RenderCopy(renderer, textTextures[47], NULL, &mainInfoTanksInfoRect);

    // Affichage des reservoirs, l'un apres l'autre
    float GapBetweenTanks = 1.05;
    Cargo cargo = ships[getWindowId()].cargo;
    SDL_Rect currentTankRect = shipFirstCompartmentRect;
    SDL_Rect currentLogoRect = shipFirstCompartmentLogoRect;
    SDL_Rect currentNumberRect = shipFirstCompartmentNumberRect;
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
            case ORE1:
                SDL_RenderCopy(renderer, imageTextures[4][1], NULL, &currentLogoRect);
                break;
            case ORE2:
                SDL_RenderCopy(renderer, imageTextures[4][2], NULL, &currentLogoRect);
                break;
            case ORE3:
                SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &currentLogoRect);
                break;
            default:
                SDL_RenderCopy(renderer, imageTextures[4][4], NULL, &currentLogoRect);
                break;
        }

        // Afficher le bord du tank
        currentTankRect.w = shipFirstCompartmentRect.w;
        SDL_DrawEdgeOfRect(currentTankRect, 3);

        // Afficher le numero du tank
        SDL_RenderCopy(renderer, textTextures[48 + i], NULL, &currentNumberRect);

        // Pour afficher le prochain reservoir
        currentTankRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
        currentLogoRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
        currentNumberRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
    }

    // Affiche 'select a new base/target' quand necessaire
    if (buttonSelected == BASE_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[45], NULL, &selectNewBaseRect);
    } else if (buttonSelected == TARGET_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[46], NULL, &selectNewTargetRect);
    }
}

void basicShipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse) {
    // Changement du bouton selectionne dans la fenetre basique d'information
    if (SDL_PointInRect(&mouse, &button1Rect)) {
        Mix_PlayChannel(1, sounds[7], 0);
        setWindowType(SHIP_WINDOW);
        buttonSelected = NO_BUTTON;
        initShipWindow(textTextures, fonts, &ships[getWindowId()]);
    } else if (SDL_PointInRect(&mouse, &button2Rect)) {
        Mix_PlayChannel(1, sounds[7], 0);
        buttonSelected = BASE_BUTTON;
    }

    // Changement de la fusee observee
    else if (SDL_PointInRect(&mouse, &rightArrowRect)) {
        Mix_PlayChannel(1, sounds[4], 0);
        setWindowId((getWindowId() + 1) % shipCount);
        initBasicShipWindow(textTextures, fonts, &ships[getWindowId()]);
        setCenterCamera((SDL_Point){ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2});
        setCameraLastObjectSelected(getWindowId());
        updateCameraFollow(ships, NULL);
        buttonSelected = NO_BUTTON;
    }
    else if (SDL_PointInRect(&mouse, &leftArrowRect)) {
        Mix_PlayChannel(1, sounds[4], 0);
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
                initBasicShipWindow(textTextures, fonts, &ships[getWindowId()]);
                Mix_PlayChannel(1, sounds[7], 0);
                break;

            case NO_BUTTON:
                if (!clickOnShip(textTextures, fonts, ships, shipCount, mouse) && 
                    !clickOnPlanet(textTextures, fonts, planets, planetCount, mouse)) {
                    if (SDL_PointInRect(&mouse, &crossRect) || !clickOnBasicShipWindow(mouse)) {
                        setWindowType(NO_WINDOW);
                        Mix_PlayChannel(1, sounds[10], 0);
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