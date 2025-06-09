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
                planetFirstResourceRect2,
                planetFirstResourceLogoRect2,
                windowLine1Rect,
                windowLine2Rect,
                windowLine3Rect,
                windowLine4Rect,
                windowLine5Rect,
                windowLine6Rect,
                windowTitleRect,
                category1TitleRect,
                category2TitleRect,
                category3TitleRect,
                category4TitleRect,
                category5TitleRect,
                category6TitleRect,
                quantityFirstOreRect,
                firstBuildRect,
                firstBarBuildRect,
                narrowAxesXAbundanceRect,
                narrowAxesYAbundanceRect,
                axesXAbundanceRect,
                axesYAbundanceRect,
                nameAxesXAbundanceRect,
                nameAxesYAbundanceRect,
                limitAxesXAbundanceRect,
                textGeneralInfoRect;


void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets) {
    initTextPlanetWindow(textTextures, fonts, planets);  // rects initialises dans le main
}

void initTextPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Stock le nom de la planete
    char planetName[64];
    generateRandomName(planetName, currentSeed, planets[getWindowId()].id);

    // Genere la texture qui donne le descriptif de la planete
    char descriptionText[512];
    sprintf(descriptionText, "Type                                 %s\nName                               %s\nDiameter                         %d km\nMass                                %d kg\nRotation period               %d d\nMain atmosphere\ncomposition                    nitrogen-oxygen\nAverage temperature    303 K\nAge                                  3.2 billion years", 
            "terrestial",
            planetName,
            (int)planets[getWindowId()].radius,
            generateRandNb32(currentSeed, 666) % 1000000,
            (int)planets[getWindowId()].orbitSpeedDeg);
    textTextures[32] = createTextTextureWithNewline(renderer, fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[32], NULL, NULL, &textureWidth, &textureHeight);
    textGeneralInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.13, SCREEN_HEIGHT * 0.22, textureWidth * SCREEN_WIDTH * 0.00027, textureHeight * SCREEN_WIDTH * 0.00027};

    // Importe la texture decrivant le titre de la page d'info. de la planete
    char planetType[32];

    switch (planets[getWindowId()].planetType) {
        case SUN:
            strcpy(planetType, "Sun");
            break;
        case PLANET:
            strcpy(planetType, "Planet");
            break;
            break;
        default:
            strcpy(planetType, "Planet type unknown...");
            break;
    }
    
    sprintf(newText.text, "%s - %s", planetType, planetName);
    newText.color = BLACK;
    newText.font = fonts[0];

    // Creation de la texture de la cible de la fusee
    updateTextTexture(&textTextures[33], newText);


    SDL_QueryTexture(textTextures[33], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = windowRect.x + windowRect.w * 0.01;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;

}

void initRectPlanetWindow(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes


    // planetWindowFoundations

    windowRect = (SDL_Rect){(1 - 0.8) * SCREEN_WIDTH / 2., (1 - 0.8) * SCREEN_HEIGHT / 2., SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.8};

    windowLine1Rect.x = windowRect.x;  // Grande barre horizontale haut
    windowLine1Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine1Rect.w = windowRect.w;
    windowLine1Rect.h = 3;

    windowLine2Rect = (SDL_Rect){SCREEN_WIDTH * 0.705, SCREEN_HEIGHT * 25/160, 3, SCREEN_WIDTH * 0.194};  // PEtite barre verticale droite haut

    windowLine3Rect.x = windowRect.x;  // Grande barre horizontale milieu
    windowLine3Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine3Rect.w = windowRect.w;
    windowLine3Rect.h = 3;

    windowLine4Rect.x = windowRect.x + windowRect.w * 0.485;  // Petite barre verticale bas gauche
    windowLine4Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine4Rect.w = 3;
    windowLine4Rect.h = windowRect.h * 0.5;

    windowLine5Rect.x = windowRect.x + windowRect.w * 0.757;  // Petite barre verticale bas droite
    windowLine5Rect.y = windowRect.y + windowRect.h * 0.5;
    windowLine5Rect.w = 3;
    windowLine5Rect.h = windowRect.h * 0.5;

    windowLine6Rect = (SDL_Rect){windowRect.x + windowRect.w * 0.485, SCREEN_HEIGHT * 25/160, 3, SCREEN_WIDTH * 31/160};  // 1ère barre verticale, 1ère ligne

    WindowCrossRect.x = windowRect.x + windowRect.w * 0.97;
    WindowCrossRect.y = windowRect.y + windowRect.h * 0.025;
    WindowCrossRect.w = windowRect.w * 0.015;
    WindowCrossRect.h = windowRect.w * 0.015;


    // planetWindowGeneralInfo
       
    SDL_QueryTexture(textTextures[52], NULL, NULL, &textureWidth, &textureHeight);
    category1TitleRect.x = windowRect.x + windowRect.w * 0.0365;
    category1TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category1TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category1TitleRect.h = textureHeight * windowRect.w * 0.0005;
    
    planetDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.22, SCREEN_WIDTH * 0.125, SCREEN_WIDTH * 0.125};


    // planetWindowContainerInfo
       
    SDL_QueryTexture(textTextures[58], NULL, NULL, &textureWidth, &textureHeight);
    category6TitleRect.x = windowRect.x + windowRect.w * 0.515;
    category6TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category6TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category6TitleRect.h = textureHeight * windowRect.w * 0.0005;

    planetFirstResourceRect = (SDL_Rect){SCREEN_WIDTH * 82/160, SCREEN_HEIGHT * 0.424, SCREEN_WIDTH * 24/160, SCREEN_WIDTH * 4/160};
    quantityFirstOreRect = (SDL_Rect){SCREEN_WIDTH * 83/160, SCREEN_HEIGHT * 0.477, SCREEN_WIDTH * 1/160, SCREEN_WIDTH * 3/160};

    planetFirstResourceLogoRect = (SDL_Rect){SCREEN_WIDTH * 107/160, SCREEN_HEIGHT * 0.424, SCREEN_WIDTH * 4/160, SCREEN_WIDTH * 4/160};


    // planetWindowMineralAbundance
       
    SDL_QueryTexture(textTextures[53], NULL, NULL, &textureWidth, &textureHeight);
    category2TitleRect.x = windowRect.x + windowRect.w * 0.785;
    category2TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category2TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category2TitleRect.h = textureHeight * windowRect.w * 0.0005;

    narrowAxesXAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.87, SCREEN_HEIGHT * 0.453, SCREEN_WIDTH * 0.01, SCREEN_WIDTH * 0.01};
    narrowAxesYAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.726, SCREEN_HEIGHT * 0.22, SCREEN_WIDTH * 0.01, SCREEN_WIDTH * 0.01};

    axesXAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.73, SCREEN_HEIGHT * 0.46, SCREEN_WIDTH * 0.147, 3};
    axesYAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.73, SCREEN_HEIGHT * 0.224, 3, SCREEN_WIDTH * 0.135};

    SDL_QueryTexture(textTextures[59], NULL, NULL, &textureWidth, &textureHeight);
    nameAxesXAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.76, SCREEN_HEIGHT * 0.465, textureWidth * SCREEN_WIDTH * 0.00027, textureHeight * SCREEN_WIDTH * 0.00027};

    SDL_QueryTexture(textTextures[60], NULL, NULL, &textureWidth, &textureHeight);
    nameAxesYAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.702, SCREEN_HEIGHT * 0.33, textureWidth * SCREEN_WIDTH * 0.00027, textureHeight * SCREEN_WIDTH * 0.00027};

    limitAxesXAbundanceRect = (SDL_Rect){SCREEN_WIDTH * 0.86, SCREEN_HEIGHT * 0.452, 5, SCREEN_WIDTH * 0.01};

    planetFirstResourceRect2 = (SDL_Rect){SCREEN_WIDTH * 0.7302, SCREEN_HEIGHT * 0.418, SCREEN_WIDTH * 0.133, SCREEN_WIDTH * 0.018};
    planetFirstResourceLogoRect2 = (SDL_Rect){SCREEN_WIDTH * 0.735, SCREEN_HEIGHT * 0.417, SCREEN_WIDTH * 0.018, SCREEN_WIDTH * 0.018};


    // planetWindowManageBuilds
       
    SDL_QueryTexture(textTextures[54], NULL, NULL, &textureWidth, &textureHeight);
    category3TitleRect.x = windowRect.x + windowRect.w * 0.037;
    category3TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category3TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category3TitleRect.h = textureHeight * windowRect.w * 0.0005;

    firstBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.144, SCREEN_HEIGHT * 0.566, SCREEN_WIDTH * 0.04, SCREEN_WIDTH * 0.04};
    firstBarBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.13, SCREEN_HEIGHT * 0.637, SCREEN_WIDTH * 0.07, SCREEN_WIDTH * 0.016};


    // planetWindowOverviewBuild
       
    SDL_QueryTexture(textTextures[55], NULL, NULL, &textureWidth, &textureHeight);
    category4TitleRect.x = windowRect.x + windowRect.w * 0.52;
    category4TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category4TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category4TitleRect.h = textureHeight * windowRect.w * 0.0005;


    // planetWindowBuildingQueue
       
    SDL_QueryTexture(textTextures[56], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.79;
    category5TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;
}


void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    planetWindowFoundations(imageTextures, textTextures);
    planetWindowGeneralInfo(imageTextures, textTextures, planets);
    planetWindowContainerInfo(imageTextures, textTextures, planets);
    planetWindowMineralAbundance(imageTextures, textTextures, planets);
    planetWindowManageBuilds(imageTextures, textTextures);
    planetWindowOverviewBuild(imageTextures, textTextures);
    planetWindowBuildingQueue(imageTextures, textTextures);
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

    // Affichage du titre de la page
    SDL_RenderCopy(renderer, textTextures[33], NULL, &windowTitleRect);
}

void planetWindowGeneralInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    // Affichage du titre "General information"
    SDL_RenderCopy(renderer, textTextures[52], NULL, &category1TitleRect);

    // Planete decrite dans cette fenetre
    int idPicture = (planets[getWindowId()].planetType == SUN) ? 9 : generateRandNb8(currentSeed, getWindowId()) % 7 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &planetDisplayedRect);  

    // Afficher le texte descriptif
    SDL_RenderCopy(renderer, textTextures[32], NULL, &textGeneralInfoRect);  
}


void planetWindowContainerInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    // Affichage du titre "Container information"
    SDL_RenderCopy(renderer, textTextures[58], NULL, &category6TitleRect);

    // Affichage des stocks de ressources, l'un apres l'autre
    SDL_Rect currentOreRect = planetFirstResourceRect;
    SDL_Rect currentLogoRect = planetFirstResourceLogoRect;
    SDL_Rect currentQuantityOreRect = quantityFirstOreRect;

    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        // Barre de fond
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        currentOreRect.w = planetFirstResourceRect.w;
        SDL_RenderFillRect(renderer, &currentOreRect);

        // Barre de niveau actuel
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        currentOreRect.w = planetFirstResourceRect.w * planets[getWindowId()].builds[i].tank.currentCapacity / planets[getWindowId()].builds[i].tank.maxCapacity;
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

        // Afficher le bord du reservoir
        currentOreRect.w = planetFirstResourceRect.w;
        SDL_DrawEdgeOfRect(renderer, currentOreRect, 3);

        // Pour afficher le prochain reservoir
        currentOreRect.y -= 1.05 * planetFirstResourceRect.h;
        currentLogoRect.y -= 1.05 * planetFirstResourceRect.h;
        currentQuantityOreRect.y -= 1.05 * planetFirstResourceRect.h;

        // Afficher nombre current ore
        renderNumber(renderer, textTextures, (int)planets[getWindowId()].builds[i].tank.currentCapacity, currentQuantityOreRect);
    }
}

void planetWindowMineralAbundance(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    // Affichage du titre "Mineral abundance"
    SDL_RenderCopy(renderer, textTextures[53], NULL, &category2TitleRect);

    // Affichage des fleches des axes du graphique
    SDL_RenderCopy(renderer, imageTextures[5][6], NULL, &narrowAxesXAbundanceRect);
    SDL_RenderCopyEx(renderer, imageTextures[5][6], NULL, &narrowAxesYAbundanceRect, -90, NULL, SDL_FLIP_NONE);

    // Affichage des axes du graphique
    SDL_RenderFillRect(renderer, &axesXAbundanceRect);
    SDL_RenderFillRect(renderer, &axesYAbundanceRect);

    // Affichage des noms des axes du graphique
    SDL_RenderCopy(renderer, textTextures[59], NULL, &nameAxesXAbundanceRect);
    SDL_RenderCopyEx(renderer, textTextures[60], NULL, &nameAxesYAbundanceRect, -90, NULL, SDL_FLIP_NONE);

    // Affichage des stocks de ressources, l'un apres l'autre
    SDL_Rect currentOreRect = planetFirstResourceRect2;
    SDL_Rect currentLogoRect = planetFirstResourceLogoRect2;

    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        // Barre de niveau actuel
        SDL_SetRenderDrawColor(renderer, 255, 95, 31, 255);
        currentOreRect.w = planetFirstResourceRect2.w * planets[getWindowId()].abundance[i] / 100;
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
                SDL_RenderCopy(renderer, imageTextures[4][0], NULL, &currentLogoRect);
                break;
        }

        // Afficher le bord du reservoir
        SDL_DrawEdgeOfRect(renderer, currentOreRect, 3);

        // Pour afficher le prochain reservoir
        currentOreRect.y -= 1.4 * planetFirstResourceRect2.h;
        currentLogoRect.y -= 1.4 * planetFirstResourceRect2.h;
    }

    // Affiche la limite de l'axe X
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &limitAxesXAbundanceRect);
}

void planetWindowManageBuilds(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "Manage builds"
    SDL_RenderCopy(renderer, textTextures[54], NULL, &category3TitleRect);

    // Affichage des differents batiments    
    SDL_Rect imageRect = firstBuildRect; 
    SDL_Rect barRect = firstBarBuildRect;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            // Affiche la barre d'amelioration/creation
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            barRect.x = firstBarBuildRect.x + firstBuildRect.w * i * 2.26;
            barRect.y = firstBarBuildRect.y + firstBuildRect.h * j * 1.5;
            SDL_RenderFillRect(renderer, &barRect);
            SDL_DrawEdgeOfRect(renderer, barRect, 2);

            // Affiche l'image de l'objet
            imageRect.x = firstBuildRect.x + firstBuildRect.w * i * 2.26;
            imageRect.y = firstBuildRect.y + firstBuildRect.h * j * 1.5;
            SDL_RenderCopy(renderer, imageTextures[9][1], NULL, &imageRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &imageRect);
        }
    }
}

void planetWindowOverviewBuild(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "Overview of build"
    SDL_RenderCopy(renderer, textTextures[55], NULL, &category4TitleRect);
}

void planetWindowBuildingQueue(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "Building queue"
    SDL_RenderCopy(renderer, textTextures[56], NULL, &category5TitleRect);
}

void planetWindowGestion(SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
        setWindowType(NO_WINDOW);
    }
}
