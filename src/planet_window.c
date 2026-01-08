#include "planet_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "renderer.h"
#include "tools.h"
#include "text.h"
#include "assets_gestion.h"
#include "window.h"
#include "place.h"


static int currentBuildIndex = 0;  
static float GapBetweenBuildX = 2.26;   
static float GapBetweenBuildY = 1.5;


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
                firstBuildImageRect,
                firstBarBuildRect,
                narrowAxesXAbundanceRect,
                narrowAxesYAbundanceRect,
                axesXAbundanceRect,
                axesYAbundanceRect,
                nameAxesXAbundanceRect,
                nameAxesYAbundanceRect,
                limitAxesXAbundanceRect,
                textGeneralInfoRect,
                firstNewTextBuildRect,
                firstUpgradeBuildRect,
                firstUpgradeBuildRect2,
                firstBuildRect,
                imageBuildRect,
                upgradeBarRect,
                titleBuildRect,
                infoBuildRect,
                updateButtonBuildRect,
                updateButtonBuildRect2,
                logoUpdateButtonBuildRect;


void initPlanetWindow(SDL_Texture **textTextures, TTF_Font **fonts, Planet *planets) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Stock le nom de la planete
    char planetName[64];
    generateRandomName(planetName, currentSeed, planets[getWindowId()].id);

    // Genere la texture qui donne le descriptif de la planete
    char descriptionText[512];
    sprintf(descriptionText, "Name                               %s\nType                                 %s\nDiameter                         %d km\nMass                                %d.%dx10^%d kg\nRotation period               %d h\nMain atmosphere\ncomposition                    nitrogen-oxygen\nAverage temperature    %d K\nAge                                  %.1f billion years", 
            "terrestial",
            planetName,
            (int)planets[getWindowId()].radius,
            (int)planets[getWindowId()].radius % 10,
            ((int)planets[getWindowId()].radius * 69) % 10,
            15 + (int)planets[getWindowId()].radius % 30,
            (int)(planets[getWindowId()].orbitSpeedDeg * 27),
            (int)(planets[getWindowId()].radius) % 1000,
            ((int)(planets[getWindowId()].radius) % 9000) / 10.f);
    textTextures[32] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
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
    windowTitleRect.x = windowRect.x + windowRect.w * 0.015;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;

    // Choix d'un nouvel objet : actualisation du nom
    SDL_QueryTexture(textTextures[66 + currentBuildIndex], NULL, NULL, &textureWidth, &textureHeight);
    titleBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.645, SCREEN_HEIGHT * 0.565, textureWidth * SCREEN_WIDTH * 0.00025, textureHeight * SCREEN_WIDTH * 0.00025};

    // Genere la texture qui donne le descriptif du batiment selectionne
    strcpy(descriptionText, "");
    if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_MINE && 
        planets[getWindowId()].builds[currentBuildIndex].mine.ore == FUEL) {
        sprintf(descriptionText, "Description:\nWork in extreme conditions\nto extract fuel, powering\n your ships for exploration.\n \nDetails: \nFunction        extraction coal\nDrain speed   %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].mine.productivity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    } 
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_MINE && 
             planets[getWindowId()].builds[currentBuildIndex].mine.ore == ORE1) {
        sprintf(descriptionText, "Description:\nIron extraction is tough\nwork, but crucial for\nbuilding what's require.\n \nDetails: \nFunction        extraction coal\nDrain speed   %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].mine.productivity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    } 
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_MINE && 
             planets[getWindowId()].builds[currentBuildIndex].mine.ore == ORE2) {
        sprintf(descriptionText, "Description:\nGold mining in remote space\nlocations is dangerous,\nbut essential for scientists.\n \nDetails: \nFunction        extraction coal\nDrain speed   %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].mine.productivity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    } 
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_MINE && 
             planets[getWindowId()].builds[currentBuildIndex].mine.ore == ORE3) {
        sprintf(descriptionText, "Description:\nJoanium is mined in high-\npressure, toxic areas,\nfor youe advanced ships.\n \nDetails: \nFunction        extraction coal\nDrain speed   %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].mine.productivity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    } 
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_MINE && 
             planets[getWindowId()].builds[currentBuildIndex].mine.ore == ORE4) {
        sprintf(descriptionText, "Description:\nMining voidor requires\nextreme effort, but it is\nneeded for high technology.\n \nDetails: \nFunction        extraction coal\nDrain speed   %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].mine.productivity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    } 
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_STORE &&
             planets[getWindowId()].builds[currentBuildIndex].tank.ore == FUEL) {
        sprintf(descriptionText, "Description:\nA secure place for fuel.\nEnsure it's full for your\nspace journeys.\n \nDetails: \nFunction        extraction coal\nStorage         %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_STORE &&
             planets[getWindowId()].builds[currentBuildIndex].tank.ore == ORE1) {
        sprintf(descriptionText, "Description:\nHolds iron for repairs and\nupgrades. Keep it ready for\nemergencies.\n \nDetails: \nFunction        extraction coal\nStorage         %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_STORE &&
             planets[getWindowId()].builds[currentBuildIndex].tank.ore == ORE2) {
        sprintf(descriptionText, "Description:\nStores gold, vital for\nbuilding rockets. It has\nunique properties.\n \nDetails: \nFunction        extraction coal\nStorage         %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_STORE &&
             planets[getWindowId()].builds[currentBuildIndex].tank.ore == ORE3) {
        sprintf(descriptionText, "Description:\nStores Joanium, a rare and\nvaluable resource for\nadvanced tech.\n \nDetails: \nFunction        extraction coal\nStorage         %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == ORE_STORE &&
             planets[getWindowId()].builds[currentBuildIndex].tank.ore == ORE4) {
        sprintf(descriptionText, "Description:\nHolds Voidor, the rarest\nmineral. You'll need more\nthan luck!\n \nDetails: \nFunction        extraction coal\nStorage         %d m3/s\nLevel              %d",
                planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity,
                planets[getWindowId()].builds[currentBuildIndex].level);
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == FACTORY) {
        sprintf(descriptionText, "Description:\nThe factory builds rockets,\nfuel tanks, and many other\nessential things.");
    }
    else if (planets[getWindowId()].builds[currentBuildIndex].type == DEFENCE_TOWER) {
        sprintf(descriptionText, "Description:\nIt's equipped with powerful\nweapons, defending all\nyour vital resources.");
    }




    textTextures[34] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[34], NULL, NULL, &textureWidth, &textureHeight);
    infoBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.516, SCREEN_HEIGHT * 0.575, (textureWidth * SCREEN_WIDTH) * 0.000232, (textureHeight * SCREEN_WIDTH) * 0.000232};
}

void initPlanetWindowRects(SDL_Texture **textTextures) {
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

    windowLine5Rect.x = windowRect.x + windowRect.w * 0.78;  // Petite barre verticale bas droite
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

    firstBuildImageRect = (SDL_Rect){SCREEN_WIDTH * 0.144, SCREEN_HEIGHT * 0.566, SCREEN_WIDTH * 0.041, SCREEN_WIDTH * 0.041};
    firstBarBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.13, SCREEN_HEIGHT * 0.637, SCREEN_WIDTH * 0.07, SCREEN_WIDTH * 0.016};

    SDL_QueryTexture(textTextures[61], NULL, NULL, &textureWidth, &textureHeight);
    firstNewTextBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.13, SCREEN_HEIGHT * 0.565, textureWidth * SCREEN_WIDTH * 0.00017, textureHeight * SCREEN_WIDTH * 0.00017};

    SDL_QueryTexture(textTextures[63], NULL, NULL, &textureWidth, &textureHeight);
    firstUpgradeBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.148, SCREEN_HEIGHT * 0.64, textureWidth * SCREEN_WIDTH * 0.0002, textureHeight * SCREEN_WIDTH * 0.0002};
    
    SDL_QueryTexture(textTextures[78], NULL, NULL, &textureWidth, &textureHeight);
    firstUpgradeBuildRect2 = (SDL_Rect){SCREEN_WIDTH * 0.158, SCREEN_HEIGHT * 0.64, textureWidth * SCREEN_WIDTH * 0.0002, textureHeight * SCREEN_WIDTH * 0.0002};
    
    firstBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.13, SCREEN_HEIGHT * 0.565, SCREEN_WIDTH * 0.07, SCREEN_WIDTH * 0.057};


    // planetWindowOverviewBuild
       
    SDL_QueryTexture(textTextures[55], NULL, NULL, &textureWidth, &textureHeight);
    category4TitleRect.x = windowRect.x + windowRect.w * 0.52;
    category4TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category4TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category4TitleRect.h = textureHeight * windowRect.w * 0.0005;

    imageBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.645, SCREEN_HEIGHT * 0.59, SCREEN_WIDTH * 0.074, SCREEN_WIDTH * 0.074};

    SDL_QueryTexture(textTextures[63], NULL, NULL, &textureWidth, &textureHeight);
    updateButtonBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.58, SCREEN_HEIGHT * 0.83, textureWidth * SCREEN_WIDTH * 0.0003, textureHeight * SCREEN_WIDTH * 0.0003};

    SDL_QueryTexture(textTextures[78], NULL, NULL, &textureWidth, &textureHeight);
    updateButtonBuildRect2 = (SDL_Rect){SCREEN_WIDTH * 0.59, SCREEN_HEIGHT * 0.83, textureWidth * SCREEN_WIDTH * 0.0003, textureHeight * SCREEN_WIDTH * 0.0003};
    
    upgradeBarRect = (SDL_Rect){SCREEN_WIDTH * 0.54, SCREEN_HEIGHT * 0.82, SCREEN_WIDTH * 0.12, SCREEN_WIDTH * 0.03};

    logoUpdateButtonBuildRect = (SDL_Rect){SCREEN_WIDTH * 0.5490, SCREEN_HEIGHT * 0.8270, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0200};


    // planetWindowNearestShips
       
    SDL_QueryTexture(textTextures[56], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.813;
    category5TitleRect.y = windowRect.y + windowRect.h * 0.53;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;
}


void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets) {
    planetWindowFoundations(imageTextures, textTextures);
    planetWindowGeneralInfo(imageTextures, textTextures, planets);
    planetWindowContainerInfo(imageTextures, textTextures, planets);
    planetWindowMineralAbundance(imageTextures, textTextures, planets);
    planetWindowManageBuilds(imageTextures, textTextures, &planets[getWindowId()]);
    planetWindowOverviewBuild(imageTextures, textTextures, &planets[getWindowId()]);
    planetWindowNearestShips(textTextures);
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
        SDL_DrawEdgeOfRect(currentOreRect, 3, BLACK);

        // Pour afficher le prochain reservoir
        currentOreRect.y -= 1.05 * planetFirstResourceRect.h;
        currentLogoRect.y -= 1.05 * planetFirstResourceRect.h;
        currentQuantityOreRect.y -= 1.05 * planetFirstResourceRect.h;

        // Afficher nombre current ore
        renderNumber((int)planets[getWindowId()].builds[i].tank.currentCapacity, currentQuantityOreRect);
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
            case ORE4:
                SDL_RenderCopy(renderer, imageTextures[4][4], NULL, &currentLogoRect);
                break;
            default:
                SDL_RenderCopy(renderer, imageTextures[4][0], NULL, &currentLogoRect);
                break;
        }

        // Afficher le bord du reservoir
        SDL_DrawEdgeOfRect(currentOreRect, 3, BLACK);

        // Pour afficher le prochain reservoir
        currentOreRect.y -= 1.4 * planetFirstResourceRect2.h;
        currentLogoRect.y -= 1.4 * planetFirstResourceRect2.h;
    }

    // Affiche la limite de l'axe X
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &limitAxesXAbundanceRect);
}

void planetWindowManageBuilds(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet) {
    // Affichage du titre "Manage builds"
    SDL_RenderCopy(renderer, textTextures[54], NULL, &category3TitleRect);

    // Affichage des differents batiments  
    SDL_Rect imageRect = firstBuildImageRect;
    SDL_Rect barRect = firstBarBuildRect;
    SDL_Rect newTextRect = firstNewTextBuildRect;
    SDL_Rect upgradeRect = firstUpgradeBuildRect;
    SDL_Rect upgradeRect2 = firstUpgradeBuildRect2;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            // Affiche du fond de la barre d'amelioration/creation
            SDL_SetRenderDrawColor(renderer, 125, 197, 46, 255);
            barRect.x = firstBarBuildRect.x + firstBuildImageRect.w * i * GapBetweenBuildX;
            barRect.y = firstBarBuildRect.y + firstBuildImageRect.h * j * GapBetweenBuildY;
            SDL_RenderFillRect(renderer, &barRect);
            SDL_DrawEdgeOfRect(barRect, 2, BLACK);

            // Affiche l'image de l'objet
            imageRect.x = firstBuildImageRect.x + firstBuildImageRect.w * i * GapBetweenBuildX;
            imageRect.y = firstBuildImageRect.y + firstBuildImageRect.h * j * GapBetweenBuildY;
            SDL_RenderCopy(renderer, imageTextures[9][i + j * 4], NULL, &imageRect);
            SDL_DrawEdgeOfRect(imageRect, 2, BLACK);

            // Affiche le texte dans la barre d'amelioration (+ logo NEW si nécessaire)
            if (planet->builds[4 * j + i].level == 0) {
                // Affiche le logo 'NEW'
                newTextRect.x = firstNewTextBuildRect.x + firstBuildImageRect.w * i * GapBetweenBuildX;
                newTextRect.y = firstNewTextBuildRect.y + firstBuildImageRect.h * j * GapBetweenBuildY;
                SDL_RenderCopy(renderer, textTextures[61], NULL, &newTextRect);

                // Affiche 'Build' dans la barre d'amélioration (batiment non construit)
                upgradeRect2.x = firstUpgradeBuildRect2.x + firstBuildImageRect.w * i * GapBetweenBuildX;
                upgradeRect2.y = firstUpgradeBuildRect2.y + firstBuildImageRect.h * j * GapBetweenBuildY;
                SDL_RenderCopy(renderer, textTextures[78], NULL, &upgradeRect2);
            } else if (planet->builds[4 * j + i].level > 0) {
                // Affiche 'Upgrade' dans la barre d'amélioration (batiment déjà construit)
                upgradeRect.x = firstUpgradeBuildRect.x + firstBuildImageRect.w * i * GapBetweenBuildX;
                upgradeRect.y = firstUpgradeBuildRect.y + firstBuildImageRect.h * j * GapBetweenBuildY;
                SDL_RenderCopy(renderer, textTextures[63], NULL, &upgradeRect);
            }

            // Logo 'Upgrade' / 'New build'
        }
    }

    // Affiche le contour du batiment choisi, dont l'apercu est affiche
    SDL_Rect edgeSelectedBuildRect = firstBuildRect;

    edgeSelectedBuildRect.x = firstBuildRect.x + firstBuildImageRect.w * (currentBuildIndex % 4) * GapBetweenBuildX;
    edgeSelectedBuildRect.y = firstBuildRect.y + firstBuildImageRect.h * (currentBuildIndex / 4) * GapBetweenBuildY;
    SDL_DrawEdgeOfRect(edgeSelectedBuildRect, 3, BLACK);
}

void planetWindowOverviewBuild(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planet) {
    // Affichage du titre "Overview of build"
    SDL_RenderCopy(renderer, textTextures[55], NULL, &category4TitleRect);

    // Afficher l'image du batiment
    SDL_DrawEdgeOfRect(imageBuildRect, 3, BLACK);
    SDL_RenderCopy(renderer, imageTextures[9][currentBuildIndex], NULL, &imageBuildRect);

    // Affichage du fond de la barre d'amelioration
    SDL_SetRenderDrawColor(renderer, 125, 197, 46, 255);
    SDL_RenderFillRect(renderer, &upgradeBarRect);
    SDL_DrawEdgeOfRect(upgradeBarRect, 3, BLACK);

    // Afficher le texte dans la barre d'amelioration
    if (planet->builds[currentBuildIndex].level == 0) {
        SDL_RenderCopy(renderer, textTextures[78], NULL, &updateButtonBuildRect2);
    } else if (planet->builds[currentBuildIndex].level > 0) {
        SDL_RenderCopy(renderer, textTextures[63], NULL, &updateButtonBuildRect);   
    }

    // Afficher le titre du batiment
    SDL_RenderCopy(renderer, textTextures[66 + currentBuildIndex], NULL, &titleBuildRect);

    // Afficher la description du batiment
    SDL_RenderCopy(renderer, textTextures[34], NULL, &infoBuildRect);

    // Afficher l'engrenage pour ameliorer un batiment
    SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &logoUpdateButtonBuildRect);
}

void planetWindowNearestShips(SDL_Texture **textTextures) {
    // Affichage du titre "Nearest ships"
    SDL_RenderCopy(renderer, textTextures[56], NULL, &category5TitleRect);
}

void planetWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship **ships, int *shipCount, Planet *planets, SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
        setWindowType(NO_WINDOW);
        Mix_PlayChannel(1, sounds[10], 0);
    }

    // Choix du batiment
    SDL_Rect edgeSelectedBuildRect = firstBuildRect;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            edgeSelectedBuildRect.x = firstBuildRect.x + firstBuildImageRect.w * i * GapBetweenBuildX;
            edgeSelectedBuildRect.y = firstBuildRect.y + firstBuildImageRect.h * j * GapBetweenBuildY;
            if (SDL_PointInRect(&mouse, &edgeSelectedBuildRect)) {
                currentBuildIndex = j * 4 + i;
                initPlanetWindow(textTextures, fonts, planets);
                Mix_PlayChannel(1, sounds[7], 0);
            }
        }
    }

    // Achat d'un nouveau batiment
    if (SDL_PointInRect(&mouse, &upgradeBarRect) && planets[getWindowId()].builds[currentBuildIndex].level == 0) {
        switch (currentBuildIndex) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                break;

            case 5:
            case 6:
            case 7:
            case 8:
            case 9:  // Acheter une mine
                if (planets[getWindowId()].builds[1].tank.currentCapacity > 100) {
                    planets[getWindowId()].builds[1].tank.currentCapacity -= 100;  // Prix a payer
                    planets[getWindowId()].builds[currentBuildIndex].level = 1;
                    Mix_PlayChannel(1, sounds[5], 0);
                }
                break;

            case 10:  // Acheter le batiment pour fabriquer des fusees
                if (planets[getWindowId()].builds[1].tank.currentCapacity > 100) {
                    planets[getWindowId()].builds[1].tank.currentCapacity -= 100;  // Prix a payer
                    planets[getWindowId()].builds[currentBuildIndex].level = 1;
                    Mix_PlayChannel(1, sounds[5], 0);
                }
                break;
            case 11:
            default:
                break;
        }
        initPlanetWindow(textTextures, fonts, planets);
    }

    // Amelioration d'un nouveau batiment
    else if (SDL_PointInRect(&mouse, &upgradeBarRect) && planets[getWindowId()].builds[currentBuildIndex].level > 0) {        
        switch (currentBuildIndex) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:  // Ameliorer un reservoir
                if (planets[getWindowId()].builds[1].tank.currentCapacity > 100) {
                    planets[getWindowId()].builds[1].tank.currentCapacity -= 100;  // Prix a payer
                    planets[getWindowId()].builds[currentBuildIndex].level ++;
                    planets[getWindowId()].builds[currentBuildIndex].tank.maxCapacity *= 1.05;
                    Mix_PlayChannel(1, sounds[5], 0);
                }
                break;

            case 5:
            case 6:
            case 7:
            case 8:
            case 9:  // Ameliorer une mine
                if (planets[getWindowId()].builds[1].tank.currentCapacity > 100) {
                    planets[getWindowId()].builds[1].tank.currentCapacity -= 100;  // Prix a payer
                    planets[getWindowId()].builds[currentBuildIndex].level ++;
                    planets[getWindowId()].builds[currentBuildIndex].mine.productivity *= 1.05;
                    Mix_PlayChannel(1, sounds[5], 0);
                }
                break;

            case 10:  // Fabriquer une nouvelle fusée
                if (planets[getWindowId()].builds[1].tank.currentCapacity > 100) {
                    planets[getWindowId()].builds[1].tank.currentCapacity -= 100;  // Prix a payer
                    addShip(ships, shipCount, &planets[getWindowId()]);
                    Mix_PlayChannel(1, sounds[5], 0);
                }
                break;
            case 11:
            default:
                break;
        }

        initPlanetWindow(textTextures, fonts, planets);
    }
}
