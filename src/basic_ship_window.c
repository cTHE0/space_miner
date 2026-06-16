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
#include "planet_window.h"


static BasicShipWindowButton buttonSelected = NO_BUTTON; // Lequel des 3 boutons est selectionne ? Au debut, aucun des boutons n'est selectionne
static SDL_Point centerBaseCoord, centerTargetCoord;
static SDL_Rect bgRect,
                shipPictureRect,
                leftArrowRect, 
                rightArrowRect,
                line1Rect,
                line2Rect,
                crossRect,
                button1Rect,
                button2Rect,
                shipTypeTitleRect,
                selectNewBaseRect,
                selectNewTargetRect,
                shipBgRect,
                generalInfoTitleRect,
                shipFirstCompartmentRect,
                shipFirstCompartmentLogoRect,
                shipFirstCompartmentNumberRect,
                logoBaseTargetRect,
                generalInfoRect,
                shipTypeImgRect,
                shipSrcRect,
                shipTypeEdgeImgRect,
                targetDisplayedRect,
                baseDisplayedRect,
                narrowRect,
                destRectShip,
                travelInfoTitleRect,
                tankCompoTitleRect,
                edgeStopButtonRect,
                edgeSwapButtonRect,
                stopButtonRect,
                stopButtonRect2,
                stopButtonRect3,
                stopButtonLogoRect,
                swapButtonLogoRect,
                swapButtonRect,
                lifeBarFullRect,
                lifeBarCurrRect;


void initBasicShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ships, Planet *planets) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Importe la texture decrivant le titre de fusee selectionnee
    sprintf(newText.text, "Basic transporter (level %d)", ships[getWindowId()].level);  // Construction "de Ship #1,887 – Basic Rocket"
    newText.color = BLACK;
    newText.font = fonts[0];

    updateTextTexture(&textTextures[33], newText);

    SDL_QueryTexture(textTextures[33], NULL, NULL, &textureWidth, &textureHeight);
    shipTypeTitleRect = (SDL_Rect){SCREEN_WIDTH * 0.29, SCREEN_HEIGHT * 0.95, textureWidth * SCREEN_HEIGHT * 0.0005, textureHeight * SCREEN_HEIGHT * 0.0005};

    // Calcul des coordonnees de la base et de la cible dans le referentiel de la map
    updateNarrowBasicShipWindow(ships, planets, &ships[getWindowId()]);

    // Genere la texture qui donne le descriptif de la fusee
    char descriptionText[512];

    char shiptype[32] = "";
    if (ships[getWindowId()].shiptype == TRANSPORTER) {
        strcpy(shiptype, "transporter");
    } else if (ships[getWindowId()].shiptype == DEFENDER) {
        strcpy(shiptype, "defender");
    } else if (ships[getWindowId()].shiptype == ENEMY) {
        strcpy(shiptype, "enemy");
    } else {
        strcpy(shiptype, "unknown");
    }

    char shipState[32] = "";
    switch (ships[getWindowId()].state) {
        case MOVING_TO_BASE:
        case MOVING_TO_TARGET:
        case MOVING_TO_BASE_SOON_STOPPED:
        case MOVING_TO_TARGET_SOON_STOPPED:
            strcpy(shipState, "in flight");
            break;
        case WAITING_ON_BASE:
        case WAITING_ON_TARGET:
        case WAITING_ON_BASE_SOON_STOPPED:
        case WAITING_ON_TARGET_SOON_STOPPED:
            strcpy(shipState, "dest. reached");
            break;
        case STOPPED_ON_BASE:
        case STOPPED_ON_TARGET:
            strcpy(shipState, "stopped");
            break;
        case OUT_OF_FUEL:
            strcpy(shipState, "out of fuel");
            break;
        case BROKE:
            strcpy(shipState, "broken");
            break;
        default:
            strcpy(shipState, "unknown");
            break;
    }
    sprintf(descriptionText, "Type of ship     %s\nState                %s\nFuel range        %d km\nMax speed        %d km/s\n \n    Health",
            shiptype,
            shipState,
            fuelInShip(&ships[getWindowId()]) / ships[getWindowId()].fuelConsumption,
            (int)(ships[getWindowId()].speed * 40));
    textTextures[34] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[34], NULL, NULL, &textureWidth, &textureHeight);
    generalInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.14, SCREEN_HEIGHT * 0.8, textureWidth * SCREEN_WIDTH * 0.00025, textureHeight * SCREEN_WIDTH * 0.00025};

    // Actualisation de la vie de la fusee dans la page d'information basique
    lifeBarCurrRect.w = (lifeBarFullRect.w * ships[getWindowId()].currentLife) / ships[getWindowId()].maxLife;
}

void updateNarrowBasicShipWindow(Ship *ships, Planet *planets, Ship *currentShip) {  // Calcul des coordonnees de la base et de la cible dans le referentiel de la map
    switch (currentShip->base.type) {
        case SPOT_PLANET:
            centerBaseCoord = (SDL_Point){planets[currentShip->base.id_planet].x, planets[currentShip->base.id_planet].y};
            break;
        case SPOT_SHIP:
            centerBaseCoord = (SDL_Point){ships[currentShip->base.id_ship].x + ships[currentShip->base.id_ship].w / 2, 
                                          ships[currentShip->base.id_ship].y + ships[currentShip->base.id_ship].h / 2};
            break;
        case SPOT_POINT:
            centerBaseCoord = currentShip->base.point;
            break;
        default:
            centerBaseCoord = (SDL_Point){1000, 1000};
            break;
    }
    switch (currentShip->target.type) {
        case SPOT_PLANET:
            centerTargetCoord = (SDL_Point){planets[currentShip->target.id_planet].x, planets[currentShip->target.id_planet].y};
            break;
        case SPOT_SHIP:
            centerTargetCoord = (SDL_Point){ships[currentShip->target.id_ship].x + ships[currentShip->target.id_ship].w / 2, 
                                            ships[currentShip->target.id_ship].y + ships[currentShip->target.id_ship].h / 2};
            break;
        case SPOT_POINT:
            centerTargetCoord = currentShip->target.point;
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

    line1Rect = (SDL_Rect){SCREEN_WIDTH * 0.4400, SCREEN_HEIGHT * 0.7500, 3, SCREEN_WIDTH * 0.1420};  // Premiere ligne verticale
    line2Rect = (SDL_Rect){SCREEN_WIDTH * 0.72, SCREEN_HEIGHT * 0.7500, 3, SCREEN_WIDTH * 0.1420};  // Premiere ligne verticale

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

    shipBgRect = (SDL_Rect){SCREEN_WIDTH * 0.6450, SCREEN_HEIGHT * 0.73, SCREEN_WIDTH * 0.3, SCREEN_WIDTH * 0.2300};

    SDL_QueryTexture(textTextures[52], NULL, NULL, &textureWidth, &textureHeight);
    generalInfoTitleRect = (SDL_Rect){SCREEN_WIDTH * 0.14, SCREEN_HEIGHT * 0.76, SCREEN_WIDTH * textureWidth * 0.0004, SCREEN_WIDTH * textureHeight * 0.0004};

    SDL_QueryTexture(textTextures[5], NULL, NULL, &textureWidth, &textureHeight);
    travelInfoTitleRect = (SDL_Rect){SCREEN_WIDTH * 0.455, SCREEN_HEIGHT * 0.76, SCREEN_WIDTH * textureWidth * 0.0004, SCREEN_WIDTH * textureHeight * 0.0004};

    SDL_QueryTexture(textTextures[14], NULL, NULL, &textureWidth, &textureHeight);
    tankCompoTitleRect = (SDL_Rect){SCREEN_WIDTH * 0.735, SCREEN_HEIGHT * 0.76, SCREEN_WIDTH * textureWidth * 0.0004, SCREEN_WIDTH * textureHeight * 0.0004};

    shipFirstCompartmentRect = (SDL_Rect){SCREEN_WIDTH * 0.78, SCREEN_HEIGHT * 0.9130, SCREEN_WIDTH * 0.0400, SCREEN_WIDTH * 0.0210};

    shipFirstCompartmentLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.755, SCREEN_HEIGHT * 0.9140, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0200};

    shipFirstCompartmentNumberRect = (SDL_Rect){SCREEN_WIDTH * 0.792, SCREEN_HEIGHT * 0.9070, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0280};

    shipTypeImgRect = (SDL_Rect){SCREEN_WIDTH * 0.3080, SCREEN_HEIGHT * 0.7700, SCREEN_WIDTH * 0.1000, SCREEN_WIDTH * 0.1000};

    shipTypeEdgeImgRect = (SDL_Rect){SCREEN_WIDTH * 0.3050, SCREEN_HEIGHT * 0.7660, SCREEN_WIDTH * 0.1100, SCREEN_WIDTH * 0.0990};

    shipSrcRect = (SDL_Rect){0, 0, 64, 64};

    targetDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 0.645, SCREEN_HEIGHT * 0.8200, SCREEN_WIDTH * 0.060, SCREEN_WIDTH * 0.060};
    baseDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 0.455, SCREEN_HEIGHT * 0.8200, SCREEN_WIDTH * 0.0600, SCREEN_WIDTH * 0.0600};

    narrowRect = (SDL_Rect){baseDisplayedRect.x + baseDisplayedRect.w, baseDisplayedRect.y + baseDisplayedRect.h * 0.5, 4, 15};

    destRectShip = (SDL_Rect){baseDisplayedRect.x + baseDisplayedRect.w, baseDisplayedRect.y + baseDisplayedRect.h * 0.13, SCREEN_WIDTH * 0.05, SCREEN_WIDTH * 0.05};

    edgeStopButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.5240, SCREEN_HEIGHT * 0.9040, SCREEN_WIDTH * 0.1150, SCREEN_WIDTH * 0.0200};

    edgeSwapButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.5240, SCREEN_HEIGHT * 0.9440, SCREEN_WIDTH * 0.1150, SCREEN_WIDTH * 0.0200};

    SDL_QueryTexture(textTextures[43], NULL, NULL, &textureWidth, &textureHeight);
    stopButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.5490, SCREEN_HEIGHT * 0.9060, SCREEN_WIDTH * textureWidth * 0.00028, SCREEN_WIDTH * textureHeight * 0.00028};
    
    SDL_QueryTexture(textTextures[64], NULL, NULL, &textureWidth, &textureHeight);
    stopButtonRect2 = (SDL_Rect){SCREEN_WIDTH * 0.5490, SCREEN_HEIGHT * 0.9060, SCREEN_WIDTH * textureWidth * 0.00028, SCREEN_WIDTH * textureHeight * 0.00028};
    
    SDL_QueryTexture(textTextures[86], NULL, NULL, &textureWidth, &textureHeight);
    stopButtonRect3 = (SDL_Rect){SCREEN_WIDTH * 0.5490, SCREEN_HEIGHT * 0.9070, SCREEN_WIDTH * textureWidth * 0.00026, SCREEN_WIDTH * textureHeight * 0.00026};
    
    stopButtonLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.5280, SCREEN_HEIGHT * 0.9090, SCREEN_WIDTH * 0.0140, SCREEN_WIDTH * 0.0140};

    swapButtonLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.5280, SCREEN_HEIGHT * 0.950, SCREEN_WIDTH * 0.0140, SCREEN_WIDTH * 0.0140};

    SDL_QueryTexture(textTextures[85], NULL, NULL, &textureWidth, &textureHeight);
    swapButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.5490, SCREEN_HEIGHT * 0.9460, SCREEN_WIDTH * textureWidth * 0.00028, SCREEN_WIDTH * textureHeight * 0.00028};

    lifeBarFullRect = (SDL_Rect){SCREEN_WIDTH * 0.1370, SCREEN_HEIGHT * 0.9640, SCREEN_WIDTH * 0.1290, SCREEN_WIDTH * 0.0100};
    lifeBarCurrRect = lifeBarFullRect;
}

void displayBasicShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets) {
    basicShipWindowNarrowBaseTarget(imageTextures, textTextures);
    basicShipWindowFondations(imageTextures);
    basicShipWindowGeneralInfo(imageTextures, textTextures, ships);
    basicShipWindowTravelInfo(imageTextures, textTextures, ships, planets);
    basicShipWindowTankCompo(imageTextures, textTextures, ships);
}

void basicShipWindowFondations(SDL_Texture ***imageTextures) {
    // Afficher le fond de la fenetre
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &bgRect);

    // Afficher les fleches pour naviguer entre les fusees
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &leftArrowRect);
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &rightArrowRect, 180, NULL, SDL_FLIP_NONE);

    // Afficher la croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);

    // Afficher les lignes structurant la page
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &line1Rect);
    SDL_RenderFillRect(renderer, &line2Rect);

    // Afficher le fond des boutons au-dessus de la fenetre
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderFillRect(renderer, &button1Rect);
    SDL_RenderFillRect(renderer, &button2Rect);

    // Afficher les boutons au-dessus de la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][4], NULL, &button1Rect);
    SDL_RenderCopy(renderer, imageTextures[2][5], NULL, &button2Rect);
}

void basicShipWindowNarrowBaseTarget(SDL_Texture ***imageTextures, SDL_Texture ** textTextures) {
    // Affichage du trait reliant base et cible
    SDL_Point centerBaseCoordScreen = {(centerBaseCoord.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                       (centerBaseCoord.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};
    SDL_Point centerTargetCoordScreen = {(centerTargetCoord.x - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f,
                                         (centerTargetCoord.y - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f};

    plotPath(centerBaseCoordScreen, centerTargetCoordScreen, 10, 5, WHITE);

    // Affichage du logo home
    float sizeLogo = getCameraScale();
    if (sizeLogo > 0.5) sizeLogo = 0.5;
    if (sizeLogo < 0.1) sizeLogo = 0.1;

    logoBaseTargetRect.w = 0.300 * SCREEN_WIDTH * sizeLogo;
    logoBaseTargetRect.h = 0.300 * SCREEN_WIDTH * sizeLogo;
    logoBaseTargetRect.x = centerBaseCoordScreen.x - logoBaseTargetRect.w / 2;
    logoBaseTargetRect.y = centerBaseCoordScreen.y - logoBaseTargetRect.w / 2;
    SDL_RenderCopy(renderer, imageTextures[5][9], NULL, &logoBaseTargetRect);

    // Affichage du logo target
    logoBaseTargetRect.x = centerTargetCoordScreen.x - logoBaseTargetRect.w / 2;
    logoBaseTargetRect.y = centerTargetCoordScreen.y - logoBaseTargetRect.w / 2;
    SDL_RenderCopy(renderer, imageTextures[5][8], NULL, &logoBaseTargetRect);

    // Afficher un logo 'target'/'home' lors de changement de destination
    logoBaseTargetRect.x = getMouseCoordinates().x - logoBaseTargetRect.w / 2;
    logoBaseTargetRect.y = getMouseCoordinates().y - logoBaseTargetRect.w / 2;
    if (buttonSelected == BASE_BUTTON) {
        SDL_RenderCopy(renderer, imageTextures[5][9], NULL, &logoBaseTargetRect);
    } else if (buttonSelected == TARGET_BUTTON) {
        SDL_RenderCopy(renderer, imageTextures[5][8], NULL, &logoBaseTargetRect);
    }

    // Affiche 'select a new base/target' quand necessaire
    if (buttonSelected == BASE_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[45], NULL, &selectNewBaseRect);
    } else if (buttonSelected == TARGET_BUTTON) {
        SDL_RenderCopy(renderer, textTextures[46], NULL, &selectNewTargetRect);
    }
}

void basicShipWindowGeneralInfo(SDL_Texture ***imageTextures, SDL_Texture ** textTextures, Ship *ships) {
    // Affiche le titre 'General information'
    SDL_RenderCopy(renderer, textTextures[52], NULL, &generalInfoTitleRect);

    // Affiche la description des informations generales
    SDL_RenderCopy(renderer, textTextures[34], NULL, &generalInfoRect);

    // Affiche la vie de la fusee (fond)
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderFillRect(renderer, &lifeBarFullRect);

    // Affiche la vie de la fusee (barre verte)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &lifeBarCurrRect);

    // Affiche la vie de la fusee (bords)
    SDL_DrawEdgeOfRect(lifeBarFullRect, 2, BLACK);

    // Afficher le nom de la fusee
    SDL_RenderCopy(renderer, textTextures[33], NULL, &shipTypeTitleRect);

    // Affiche le fond de la fusee selectionnee
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderFillRect(renderer, &shipTypeEdgeImgRect);

    // Affiche le bord de la fusee selectionnee
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &shipTypeEdgeImgRect);

    // Affiche l'image du model de fusee selectionne
    SDL_RenderCopy(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &shipTypeImgRect);
}

void basicShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture ** textTextures, Ship *ships, Planet *planets) {
    // Affiche le titre 'Travel information'
    SDL_RenderCopy(renderer, textTextures[5], NULL, &travelInfoTitleRect);

    // Affichage des deux planetes
    int idPicture;
    if (ships[getWindowId()].base.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].base.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].base.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &baseDisplayedRect);
    } else if (ships[getWindowId()].base.type == SPOT_POINT) {
        SDL_RenderCopy(renderer, imageTextures[5][13], NULL, &baseDisplayedRect);
    }

    if (ships[getWindowId()].target.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].target.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].target.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &targetDisplayedRect);
    } else if (ships[getWindowId()].target.type == SPOT_POINT) {
        SDL_RenderCopy(renderer, imageTextures[5][13], NULL, &targetDisplayedRect);
    }

    // Affichage bouton stop 
    if (ships[getWindowId()].state == STOPPED_ON_BASE || ships[getWindowId()].state == STOPPED_ON_TARGET) {  // Bouton redemarrage
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        SDL_RenderFillRect(renderer, &edgeStopButtonRect);
        SDL_RenderCopy(renderer, textTextures[64], NULL, &stopButtonRect2);
    } else if (ships[getWindowId()].state == WAITING_ON_BASE || ships[getWindowId()].state == WAITING_ON_TARGET) { 
        SDL_RenderCopy(renderer, textTextures[86], NULL, &stopButtonRect3);
    } else {
        if (ships[getWindowId()].state == MOVING_TO_BASE_SOON_STOPPED || ships[getWindowId()].state == MOVING_TO_TARGET_SOON_STOPPED ||
            ships[getWindowId()].state == WAITING_ON_BASE_SOON_STOPPED || ships[getWindowId()].state == WAITING_ON_BASE_SOON_STOPPED) {  // Trace du fond vert lorsque l'arret est prevu
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
            SDL_RenderFillRect(renderer, &edgeStopButtonRect);
        }
        SDL_RenderCopy(renderer, textTextures[43], NULL, &stopButtonRect);
    }
    SDL_DrawEdgeOfRect(edgeStopButtonRect, 3, BLACK);
    SDL_RenderCopy(renderer, imageTextures[2][8], NULL, &stopButtonLogoRect);  // Logo

    // Affichage du bouton pour echanger la destination (passer de target a base)
    SDL_DrawEdgeOfRect(edgeSwapButtonRect, 3, BLACK);
    SDL_RenderCopy(renderer, textTextures[85], NULL, &swapButtonRect);
    SDL_RenderCopy(renderer, imageTextures[5][11], NULL, &swapButtonLogoRect);  // Logo

    // Affichage du systeme de progression de la fusee dans l'espace
    int dp = targetDisplayedRect.x - baseDisplayedRect.x - baseDisplayedRect.w;  // Distance en pixel entre 2 planetes sur fenetre
    
    if (ships[getWindowId()].shiptype == TRANSPORTER) {   
        // Fraction du chemin parcourue
        float f;

        SDL_Point pointBase;  // Variable temporaire
        float shiftBase = 0;
        if (ships[getWindowId()].base.type == SPOT_PLANET) {
            pointBase = (SDL_Point){planets[ships[getWindowId()].base.id_planet].x, planets[ships[getWindowId()].base.id_planet].y};
            shiftBase = planets[ships[getWindowId()].base.id_planet].radius;
        } else if (ships[getWindowId()].base.type == SPOT_POINT) {
            pointBase = ships[getWindowId()].base.point;
        } else {
            pointBase = (SDL_Point){0, 0};
        }

        SDL_Point pointTarget;  // Variable temporaire
        float shiftTarget = 0;
        if (ships[getWindowId()].target.type == SPOT_PLANET) {
            pointTarget = (SDL_Point){planets[ships[getWindowId()].target.id_planet].x, planets[ships[getWindowId()].target.id_planet].y};
            shiftTarget = planets[ships[getWindowId()].target.id_planet].radius;
        } else if (ships[getWindowId()].target.type == SPOT_POINT) {
            pointTarget = ships[getWindowId()].target.point;
        } else {
            pointTarget = (SDL_Point){0, 0};
        }

        f = (dist(pointBase.x, pointBase.y, ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2) - shiftBase)
            / (distancePointPoint(&pointBase, &pointTarget) - shiftTarget - shiftBase);
        if (f > 1) f = 1;
        else if (f < 0) f = 0;

        if (ships[getWindowId()].state == MOVING_TO_TARGET || ships[getWindowId()].state == WAITING_ON_BASE ||
            ships[getWindowId()].state == MOVING_TO_TARGET_SOON_STOPPED || ships[getWindowId()].state == WAITING_ON_BASE_SOON_STOPPED) { 

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
            SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &destRectShip, 90, NULL, SDL_FLIP_NONE);

        } else if (ships[getWindowId()].state == MOVING_TO_BASE || ships[getWindowId()].state == WAITING_ON_TARGET ||
                   ships[getWindowId()].state == MOVING_TO_BASE_SOON_STOPPED || ships[getWindowId()].state == WAITING_ON_TARGET_SOON_STOPPED) {
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
            SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &destRectShip, 270, NULL, SDL_FLIP_NONE);
        } else {
            // Tracer la fleche
            narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            while (narrowRect.x < targetDisplayedRect.x) {
                SDL_RenderFillRect(renderer, &narrowRect);
                narrowRect.x += narrowRect.w * 2;
            }

            // Tracer la fusee
            destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w + f * dp - baseDisplayedRect.w * 0.53;
            if (ships[getWindowId()].state == STOPPED_ON_BASE) {
                SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &destRectShip, 90, NULL, SDL_FLIP_NONE);
            } else if (ships[getWindowId()].state == STOPPED_ON_TARGET) {
                SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &destRectShip, 270, NULL, SDL_FLIP_NONE);
            } else {
                SDL_RenderCopy(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipSrcRect, &destRectShip);
            }
        }
    }
}

void basicShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture ** textTextures, Ship *ships) {
    // Affiche le titre 'Tank composition'
    SDL_RenderCopy(renderer, textTextures[14], NULL, &tankCompoTitleRect);

    // Affiche la fusee en arriere plan (reservoir par dessus)
    SDL_RenderCopy(renderer, imageTextures[5][7], &shipSrcRect, &shipBgRect);

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
            case ORE4:
                SDL_RenderCopy(renderer, imageTextures[4][4], NULL, &currentLogoRect);
                break;
            default:
                break;
        }

        // Afficher le bord du tank
        currentTankRect.w = shipFirstCompartmentRect.w;
        SDL_DrawEdgeOfRect(currentTankRect, 3, BLACK);

        // Afficher le numero du tank
        SDL_RenderCopy(renderer, textTextures[48 + i], NULL, &currentNumberRect);

        // Pour afficher le prochain reservoir
        currentTankRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
        currentLogoRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
        currentNumberRect.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
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
        if (buttonSelected == BASE_BUTTON) {
            buttonSelected = TARGET_BUTTON;
        } else if (buttonSelected == TARGET_BUTTON) {
            buttonSelected = NO_BUTTON;
        } else {
            buttonSelected = BASE_BUTTON;
        }
    }  

    // Changement de la fusee observee
    else if (SDL_PointInRect(&mouse, &rightArrowRect)) {
        // Changement de l'id de la page
        setWindowId((getWindowId() + 1) % shipCount);
        while (ships[getWindowId()].shiptype != TRANSPORTER) setWindowId((getWindowId() + 1) % shipCount);

        initBasicShipWindow(textTextures, fonts, ships, planets);
        setCenterCamera((SDL_Point){ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2});
        setCameraLastObjectSelected(getWindowId());
        updateCameraFollow(ships, NULL);
        buttonSelected = NO_BUTTON;
        Mix_PlayChannel(1, sounds[4], 0);
    }
    else if (SDL_PointInRect(&mouse, &leftArrowRect)) {
        // Changement de l'id de la page
        if (getWindowId() == 0) {
            setWindowId(shipCount - 1);
        } else {
            setWindowId(getWindowId() - 1);
        }
        while (ships[getWindowId()].shiptype != TRANSPORTER) {
            if (getWindowId() == 0) {
                setWindowId(shipCount - 1);
            } else {
                setWindowId(getWindowId() - 1);
            }
        }

        initBasicShipWindow(textTextures, fonts, ships, planets);
        setCenterCamera((SDL_Point){ships[getWindowId()].x + ships[getWindowId()].w / 2, ships[getWindowId()].y + ships[getWindowId()].h / 2});
        setCameraLastObjectSelected(getWindowId());
        updateCameraFollow(ships, NULL);
        Mix_PlayChannel(1, sounds[4], 0);
    }

    // Ouverture de la fenetre de la planete (depuis la fenetre d'info. de la fusee)
    else if (SDL_PointInRect(&mouse, &baseDisplayedRect)) {
        if (ships[getWindowId()].base.type == SPOT_PLANET) {
            initPlanetWindow(textTextures, fonts, planets, ships, shipCount);
            setCameraLastObjectSelected(ships[getWindowId()].base.id_planet);
            setCameraMode(FOLLOW_PLANET);
            setWindowType(PLANET_WINDOW);
            setWindowId(ships[getWindowId()].base.id_planet);
            Mix_PlayChannel(0, sounds[3], 0);
        }
    }
    else if (SDL_PointInRect(&mouse, &targetDisplayedRect)) {
        if (ships[getWindowId()].target.type == SPOT_PLANET) {
            initPlanetWindow(textTextures, fonts, planets, ships, shipCount);
            setCameraLastObjectSelected(ships[getWindowId()].target.id_planet);
            setCameraMode(FOLLOW_PLANET);
            setWindowType(PLANET_WINDOW);
            setWindowId(ships[getWindowId()].target.id_planet);
            Mix_PlayChannel(0, sounds[3], 0);
        }
    }

    // Systeme d'arret des moteurs/du remplissage de la fusee 
    else if (SDL_PointInRect(&mouse, &edgeStopButtonRect)) {
        // La fusee s'arretera au prochain arret (apres avoir transfere ses minerais)
        if (ships[getWindowId()].state == MOVING_TO_BASE) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = MOVING_TO_BASE_SOON_STOPPED;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        } else if(ships[getWindowId()].state == MOVING_TO_TARGET) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = MOVING_TO_TARGET_SOON_STOPPED;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        } else if (ships[getWindowId()].state == MOVING_TO_BASE_SOON_STOPPED) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = MOVING_TO_BASE;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        } else if(ships[getWindowId()].state == MOVING_TO_TARGET_SOON_STOPPED) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = MOVING_TO_TARGET;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        }

        // Arrêt des échanges de ressources entre la fusée et la planète
        else if (ships[getWindowId()].state == WAITING_ON_BASE) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = STOPPED_ON_BASE;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        } 
        else if (ships[getWindowId()].state == WAITING_ON_TARGET) {
            Mix_PlayChannel(1, sounds[8], 0);
            ships[getWindowId()].state = STOPPED_ON_TARGET;
            initBasicShipWindow(textTextures, fonts, ships, planets);
        }

        // Redemarrage des moteur (apres avoir transfere ses minerais)
        else if (ships[getWindowId()].state == STOPPED_ON_BASE) {
            Mix_PlayChannel(1, sounds[8], 0);
            if (ships[getWindowId()].base.type == SPOT_PLANET) {
                ships[getWindowId()].state = WAITING_ON_BASE;  // Pour recuperer les ressources avant de partir de la planete
            } else {
                ships[getWindowId()].state = MOVING_TO_TARGET;
            }
            initBasicShipWindow(textTextures, fonts, ships, planets);
        } else if (ships[getWindowId()].state == STOPPED_ON_TARGET) {
            Mix_PlayChannel(1, sounds[8], 0);
            if (ships[getWindowId()].base.type == SPOT_PLANET) {
                ships[getWindowId()].state = WAITING_ON_TARGET;  // Pour recuperer les ressources avant de partir de la planete
            } else {
                ships[getWindowId()].state = MOVING_TO_BASE;
            }
            initBasicShipWindow(textTextures, fonts, ships, planets);
        }

        // Arret du remplissage des reservoirs
        else if (ships[getWindowId()].state == WAITING_ON_BASE || ships[getWindowId()].state == WAITING_ON_TARGET) {
            
        }
    }

    // Echange de la destination si la fusee est en vol
    else if (SDL_PointInRect(&mouse, &swapButtonRect)) {
        if (ships[getWindowId()].state == MOVING_TO_BASE) {
            ships[getWindowId()].state = MOVING_TO_TARGET;
        } else if (ships[getWindowId()].state == MOVING_TO_TARGET) {
            ships[getWindowId()].state = MOVING_TO_BASE;
        } else if (ships[getWindowId()].state == MOVING_TO_BASE_SOON_STOPPED) {
            ships[getWindowId()].state = MOVING_TO_TARGET_SOON_STOPPED;
        } else if (ships[getWindowId()].state == MOVING_TO_TARGET_SOON_STOPPED) {
            ships[getWindowId()].state = MOVING_TO_BASE_SOON_STOPPED;
        }
    }
    
    // Fermeture de la fenêtre basique de la fusée
    else if (SDL_PointInRect(&mouse, &crossRect) || 
        (buttonSelected == NO_BUTTON && !clickOnShip(textTextures, fonts, ships, shipCount, planets, mouse) && !clickOnPlanet(textTextures, fonts, planets, ships, planetCount, shipCount, mouse) && !clickOnBasicShipWindow(mouse))) {
        setWindowType(NO_WINDOW);
        Mix_PlayChannel(1, sounds[10], 0);
    }

    // Changement de la destination de la fusée.
    else if (buttonSelected == BASE_BUTTON || buttonSelected == TARGET_BUTTON) {
        choosingNewBaseOrTarget(ships, shipCount, planets, planetCount, mouse);
        initBasicShipWindow(textTextures, fonts, ships, planets);
        Mix_PlayChannel(1, sounds[7], 0);

    }
}

void choosingNewBaseOrTarget(Ship *ships, int shipCount, Planet *planets, int planetCount, SDL_Point mouse) {
    int planetChosen = whichPlanetIsClicked(planets, planetCount, mouse);
    int shipChosen = whichShipIsClicked(ships, shipCount, mouse);
    Spot *spotDest = ((buttonSelected == BASE_BUTTON) ? &ships[getWindowId()].base : &ships[getWindowId()].target);

    if (planetChosen != -1) {  // La nouvelle cible est une planete
        spotDest->type = SPOT_PLANET;
        spotDest->id_planet = planetChosen;
    } else if (ships[getWindowId()].shiptype == ENEMY && shipChosen != -1) {  // La nouvelle cible est un ship (reserve pour les fusees offensives)
        spotDest->type = SPOT_SHIP;
        spotDest->id_ship = shipChosen;
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