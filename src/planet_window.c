#include "planet_window.h"

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "renderer.h"
#include "tools.h"
#include "text.h"
#include "assets_gestion.h"
#include "window.h"
#include "place.h"


// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static SDL_Rect windowRect,
                WindowCrossRect,
                planetDisplayedRect,
                planetFirstResourceRect,
                planetFirstResourceLogoRect,
                windowLine1Rect,
                windowLine2Rect,
                windowLine3Rect,
                windowLine4Rect,
                windowLine5Rect,
                windowLine6Rect,
                square1Rect,
                square2Rect,
                square3Rect,
                windowTitleRect,
                category1TitleRect,
                category2TitleRect,
                category3TitleRect,
                category4TitleRect,
                category5TitleRect,
                nbFirstOreRect;


void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets) {
    initTextPlanetWindow(textTextures, fonts, planets);  // rects initialises dans le main
}

void initTextPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Importe la texture decrivant le titre de la page d'info. de la planete
    char name_planet[64];
    generateRandomName(name_planet, currentSeed, planets[getWindowId()].id);


    char type_planet[32];
    switch (planets[getWindowId()].planetType) {
        case SUN:
            strcpy(type_planet, "Sun");
            break;
        case PLANET:
            strcpy(type_planet, "Planet");
            break;
            break;
        default:
            strcpy(type_planet, "Planet type unknown...");
            break;
    }
    
    sprintf(newText.text, "%s - %s", type_planet, name_planet);
    newText.color = BLACK;
    newText.font = fonts[0];

    updateTextTexture(&textTextures[33], newText);

    SDL_QueryTexture(textTextures[33], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = windowRect.x + windowRect.w * 0.01;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;
}

void initRectPlanetWindow(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes


    //planetWindowFoundations

    windowRect.x = (1 - 0.8) * SCREEN_WIDTH / 2.;
    windowRect.y = (1 - 0.8) * SCREEN_HEIGHT / 2.;
    windowRect.w = SCREEN_WIDTH * 0.8;
    windowRect.h = SCREEN_HEIGHT * 0.8;

    windowLine1Rect.x = windowRect.x;  // Grande barre horizontale haut
    windowLine1Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine1Rect.w = windowRect.w;
    windowLine1Rect.h = 3;

    windowLine2Rect = (SDL_Rect){SCREEN_WIDTH * 108/160, SCREEN_HEIGHT * 25/160, 3, SCREEN_WIDTH * 37/160};

    windowLine3Rect.x = windowRect.x;  // Grande barre horizontale milieu
    windowLine3Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine3Rect.w = windowRect.w;
    windowLine3Rect.h = 3;

    windowLine4Rect.x = windowRect.x + windowRect.w * 0.45;  // Grande barre verticale bas gauche
    windowLine4Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine4Rect.w = 3;
    windowLine4Rect.h = windowRect.h * 0.5;

    windowLine5Rect.x = windowRect.x + windowRect.w * 0.72;  // Grande barre verticale bas droite
    windowLine5Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine5Rect.w = 3;
    windowLine5Rect.h = windowRect.h * 0.5;

    //1ère barre verticale, 1ère ligne
    windowLine6Rect = (SDL_Rect){windowRect.x + windowRect.w * 0.45, SCREEN_HEIGHT * 25/160, 3, SCREEN_WIDTH * 37/160};

    square1Rect = (SDL_Rect){SCREEN_WIDTH *21/160, SCREEN_HEIGHT * 103/160, SCREEN_WIDTH * 15/160, SCREEN_WIDTH * 19/160};
    square2Rect = (SDL_Rect){SCREEN_WIDTH *37/160, SCREEN_HEIGHT * 103/160, SCREEN_WIDTH * 15/160, SCREEN_WIDTH * 19/160};
    square3Rect = (SDL_Rect){SCREEN_WIDTH *53/160, SCREEN_HEIGHT * 103/160, SCREEN_WIDTH * 15/160, SCREEN_WIDTH * 19/160};

    WindowCrossRect.x = windowRect.x + windowRect.w * 0.97;
    WindowCrossRect.y = windowRect.y + windowRect.h * 0.025;
    WindowCrossRect.w = windowRect.w * 0.015;
    WindowCrossRect.h = windowRect.w * 0.015;


    // planetWindowGeneralInfo
       
    SDL_QueryTexture(textTextures[52], NULL, NULL, &textureWidth, &textureHeight);
    category1TitleRect.x = windowRect.x + windowRect.w * 0.05;
    category1TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category1TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category1TitleRect.h = textureHeight * windowRect.w * 0.0005;
    
    planetDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 52/160, SCREEN_HEIGHT * 40/160, SCREEN_WIDTH * 18/160, SCREEN_WIDTH * 17/160};


    // planetWindowOverviewBuildings
       
    SDL_QueryTexture(textTextures[53], NULL, NULL, &textureWidth, &textureHeight);
    category2TitleRect.x = windowRect.x + windowRect.w * 0.77;
    category2TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category2TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category2TitleRect.h = textureHeight * windowRect.w * 0.0005;



    // planetWindowNewBuildings
       
    SDL_QueryTexture(textTextures[54], NULL, NULL, &textureWidth, &textureHeight);
    category3TitleRect.x = windowRect.x + windowRect.w * 0.05;
    category3TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category3TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category3TitleRect.h = textureHeight * windowRect.w * 0.0005;



    // planetWindowOreAbundance
       
    SDL_QueryTexture(textTextures[55], NULL, NULL, &textureWidth, &textureHeight);
    category4TitleRect.x = windowRect.x + windowRect.w * 0.49;
    category4TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category4TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category4TitleRect.h = textureHeight * windowRect.w * 0.0005;



    // planetWindowBuildingQueue
       
    SDL_QueryTexture(textTextures[56], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.79;
    category5TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;

    planetFirstResourceRect = (SDL_Rect){SCREEN_WIDTH * 77/160, SCREEN_HEIGHT * 65/160, SCREEN_WIDTH * 24/160, SCREEN_WIDTH * 5/160};
    nbFirstOreRect = (SDL_Rect){SCREEN_WIDTH * 80/160, SCREEN_HEIGHT * 74/160, SCREEN_WIDTH * 1/160, SCREEN_WIDTH * 3/160};

    planetFirstResourceLogoRect = (SDL_Rect){SCREEN_WIDTH * 102/160, SCREEN_HEIGHT * 65/160, SCREEN_WIDTH * 4/160, SCREEN_WIDTH * 4/160};
}


void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount) {
    planetWindowFoundations(imageTextures, textTextures);
    planetWindowGeneralInfo(imageTextures, textTextures, planets);
    planetWindowOverviewBuildings(imageTextures, textTextures);
    planetWindowNewBuildings(imageTextures, textTextures);
    planetWindowBuildingQueue(imageTextures, textTextures);
    planetWindowOreAbundance(imageTextures, textTextures, planets);
}

void planetWindowFoundations(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Arriere plan de la fenetre d'informations
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &windowRect);

    // Croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect); 

    // Barres pour structurer la page 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &windowLine1Rect);
    SDL_RenderFillRect(renderer, &windowLine2Rect);
    SDL_RenderFillRect(renderer, &windowLine3Rect);
    SDL_RenderFillRect(renderer, &windowLine4Rect);
    SDL_RenderFillRect(renderer, &windowLine5Rect);
    SDL_RenderFillRect(renderer, &windowLine6Rect);

    // Carrée affichage bâtiments
    SDL_RenderCopy(renderer, imageTextures[5][7], NULL, &square1Rect);
    SDL_RenderCopy(renderer, imageTextures[5][7], NULL, &square2Rect);
    SDL_RenderCopy(renderer, imageTextures[5][7], NULL, &square3Rect);

    // Affichage du titre de la page
    SDL_RenderCopy(renderer, textTextures[33], NULL, &windowTitleRect);
}

void planetWindowGeneralInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[52], NULL, &category1TitleRect);

    // Planete decrite dans cette fenetre
    int idPicture = (planets[getWindowId()].planetType == SUN) ? 9 : generateRandNb8(currentSeed, getWindowId()) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &planetDisplayedRect);  
}

void planetWindowOverviewBuildings(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[53], NULL, &category2TitleRect);
}

void planetWindowNewBuildings(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[54], NULL, &category3TitleRect);
}

void planetWindowBuildingQueue(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[55], NULL, &category4TitleRect);
}

void planetWindowOreAbundance(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[56], NULL, &category5TitleRect);
    // Affichage des stocks de ressources, l'un apres l'autre
    SDL_Rect currentOreRect = planetFirstResourceRect;
    SDL_Rect currentLogoRect = planetFirstResourceLogoRect;
    SDL_Rect currentNbOreRect = nbFirstOreRect;

    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        // Barre de fond
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        currentOreRect.w = planetFirstResourceRect.w;
        SDL_RenderFillRect(renderer, &currentOreRect);

        // Barre de niveau actuel
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        currentOreRect.w = planetFirstResourceRect.w * planets[getWindowId()].currentOre[i] / planets[getWindowId()].maxOre[i];
        SDL_RenderFillRect(renderer, &currentOreRect);

        // Logo du type de minerai
        switch (i) {
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

        // Pour afficher le prochain reservoir
        currentOreRect.y -= 1.05 * planetFirstResourceRect.h;
        currentLogoRect.y -= 1.05 * planetFirstResourceRect.h;
        currentNbOreRect.y -= 1.05 * planetFirstResourceRect.h;

        // Afficher nombre current ore
        renderNumber(renderer, textTextures, (int)planets[getWindowId()].currentOre[i], currentNbOreRect, 8);
    }
}

void planetWindowGestion(SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
        setWindowType(NO_WINDOW);
    }
}
