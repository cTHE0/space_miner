#include "ship_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "assets_gestion.h"
#include "renderer.h"
#include "tools.h"
#include "window.h"
#include "planet_window.h"
#include "basic_ship_window.h"
#include "place.h"
#include "camera.h"


// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static char baseName[64] = {0};
static char targetName[64] = {0};
static int currentTankIndex = 0;
static int currentOreParameterIndex = -1;

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
                windowLine5Rect,
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
                shipFirstCompartmentLogoRect,
                shipFirstCompartmentNumberRect,
                flowInTankRightRect,
                flowInTankLeftRect,
                shipRightRect,
                shipRightRect2,
                shipLeftRect,
                oreToTransfert1Rect,
                oreToTransfert2Rect,
                oreToTransfert3Rect,
                oreToTransfert4Rect,
                changeTankManagerLeft,
                changeTankManagerRight,
                changeTankManagerLeft2,
                changeTankManagerRight2,
                orePossibility0,
                orePossibility1,
                orePossibility2,
                orePossibility3,
                orePossibility4,
                orePossibility5,
                maxLifeShipRect,
                currentLifeShipRect,
                stopButtonRect,
                stopButtonRect2,
                notEnoughFuelRect,
                infoPerTankRect,
                edgeStopButtonRect,
                edgeSwapButtonRect,
                edgeTankChoosenButtonRect,
                edgeTankChoosenButtonRect2,
                mainInfoTanksEdgeRect,
                mainInfoTanksInfoRect,
                tankInfoRect,
                upgradeButtonTankRect,
                upgradeButtonTankRect2,
                repareShipButtonRect,
                repareShipButtonRect2,
                upgradeShipButtonRect,
                upgradeShipButtonRect2,
                percentShipHealthRect,
                shipInfoRect,
                shipInfoRect2,
                logoUpgradeButtonShipInfoRect,
                logoUpgradeButtonTankInfoRect,
                logoRepairButtonShipInfoRect,
                currentVisualNarrowRect,
                stopButtonLogoRect,
                swapButtonLogoRect,
                swapButtonRect;


void initShipWindow(SDL_Texture **textTextures, TTF_Font **fonts, Ship *ship) {
    int textureWidth, textureHeight;
    TextToLoad newText;

    // Importe le nom de la base de la fusee
    if (ship->base.type == SPOT_PLANET) {
        generateRandomName(baseName, currentSeed, ship->base.id_planet);
        strcpy(newText.text, (const char*)baseName);
        newText.color = BLACK;
        newText.font = fonts[0];
    
        // Creation de la texture de la base de la fusee
        updateTextTexture(&textTextures[33], newText);

        // Atualisation de la taille du rect affichant ce nom
        SDL_QueryTexture(textTextures[33], NULL, NULL, &textureWidth, &textureHeight);
        nameBaseDisplayedRect.x = windowRect.x + windowRect.w * 0.03;
        nameBaseDisplayedRect.y = windowRect.y + windowRect.h * 0.38;
        nameBaseDisplayedRect.w = textureWidth * windowRect.w * 0.0004;
        nameBaseDisplayedRect.h = textureHeight * windowRect.w * 0.0004;
    }

    if (ship->target.type == SPOT_PLANET) {
        // Importe le nom de la cible de la fusee
        generateRandomName(targetName, currentSeed, ship->target.id_planet);
        strcpy(newText.text, (const char*)targetName);
        newText.color = BLACK;
        newText.font = fonts[0];

        // Creation de la texture de la cible de la fusee
        updateTextTexture(&textTextures[34], newText);

        // Atualisation de la taille du rect affichant ce nom
        SDL_QueryTexture(textTextures[34], NULL, NULL, &textureWidth, &textureHeight);
        nameTargetDisplayedRect.x = windowRect.x + windowRect.w * 0.43;
        nameTargetDisplayedRect.y = windowRect.y + windowRect.h * 0.38;
        nameTargetDisplayedRect.w = textureWidth * windowRect.w * 0.0004;
        nameTargetDisplayedRect.h = textureHeight * windowRect.w * 0.0004;
    }

    // Importe la texture decrivant le titre de la page d'info. de la fusee
    sprintf(newText.text, "Ship #%d – Basic Rocket", getWindowId() + 1);  // Construction "de Ship #1,887 – Basic Rocket"
    newText.color = BLACK;
    newText.font = fonts[0];

    updateTextTexture(&textTextures[32], newText);

    SDL_QueryTexture(textTextures[32], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = windowRect.x + windowRect.w * 0.01;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;

    // Genere la texture qui donne le descriptif de la planete
    char descriptionText[512];
    sprintf(descriptionText, "Countenance     fuel\nFill rate              %d %%\nCapacity            %d m3\nDrain speed       %d\nUpgrade level    %d", 
            (ship->cargo.compartmentsList[currentTankIndex].currentCapacity * 100) / ship->cargo.compartmentsList[currentTankIndex].maxCapacity,
            ship->cargo.compartmentsList[currentTankIndex].maxCapacity,
            ship->cargo.compartmentsList[currentTankIndex].flowSpeed,
            ship->cargo.compartmentsList[currentTankIndex].level);
    textTextures[35] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[35], NULL, NULL, &textureWidth, &textureHeight);
    tankInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.567, SCREEN_HEIGHT * 0.69, textureWidth * SCREEN_WIDTH * 0.00025, textureHeight * SCREEN_WIDTH * 0.00025};

    // Mise a jour de la vie de la fusee
    strcpy(descriptionText, "");
    sprintf(descriptionText, "%d%%", (ship->currentLife * 100) / ship->maxLife);
    textTextures[36] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[36], NULL, NULL, &textureWidth, &textureHeight);
    percentShipHealthRect = (SDL_Rect){SCREEN_WIDTH * 0.747, SCREEN_HEIGHT * 0.245, SCREEN_WIDTH * textureWidth * 0.0002, SCREEN_WIDTH * textureHeight * 0.0002};
    
    // Mise a jour des donnees de la fusee (description de gauche)
    strcpy(descriptionText, "");
    sprintf(descriptionText, "Model               Basic Rocket\nProduction No %d\nLevel                %d\nMove speed     %.2f km/s",
            getWindowId() + 1,
            ship->level,
            ship->speed);

    textTextures[37] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[37], NULL, NULL, &textureWidth, &textureHeight);
    shipInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.565, SCREEN_HEIGHT * 0.4, SCREEN_WIDTH * textureWidth * 0.00025, SCREEN_WIDTH * textureHeight * 0.00025};    
    
    // Mise a jour des donnees de la fusee (description de droite)
    strcpy(descriptionText, "");
    sprintf(descriptionText, "Fuel consumption       %d L/s  \nTank capacity            %.2f m3\nDurability                    2h \nMineral transferred  199,552 m3",
        ship->fuelConsumption,
        globalTankCapacity(ship) / 100.f
        );

    textTextures[38] = createTextTextureWithNewline(fonts[0], descriptionText, BLACK);
    SDL_QueryTexture(textTextures[38], NULL, NULL, &textureWidth, &textureHeight);
    shipInfoRect2 = (SDL_Rect){SCREEN_WIDTH * 0.72, SCREEN_HEIGHT * 0.4, SCREEN_WIDTH * textureWidth * 0.00025, SCREEN_WIDTH * textureHeight * 0.00025};

    // Met a jour le tank en cours de modification/observation
    SDL_QueryTexture(textTextures[15 + currentTankIndex], NULL, NULL, &textureWidth, &textureHeight);
    modifyingTankRect.x = windowRect.x + windowRect.w * 0.25;
    modifyingTankRect.y = windowRect.y + windowRect.h * 0.55;
    modifyingTankRect.w = textureWidth * windowRect.w * 0.0005;
    modifyingTankRect.h = textureHeight * windowRect.w * 0.0005;

    SDL_QueryTexture(textTextures[15 + currentTankIndex], NULL, NULL, &textureWidth, &textureHeight);
    infoPerTankRect.x = windowRect.x + windowRect.w * 0.663;
    infoPerTankRect.y = windowRect.y + windowRect.h * 0.68;
    infoPerTankRect.w = textureWidth * windowRect.w * 0.00035;
    infoPerTankRect.h = textureHeight * windowRect.w * 0.00035;
}

void initShipWindowRects(SDL_Texture **textTextures) {  // Les rects sont initialises dans assets_gestion.c
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

    windowLine1Rect.x = windowRect.x;  // Grande barre horizontale haut
    windowLine1Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine1Rect.w = windowRect.w;
    windowLine1Rect.h = 3;

    windowLine2Rect.x = windowRect.x;  // Barre horizontale gauche milieu
    windowLine2Rect.y = windowRect.y + windowRect.h * 0.45;
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

    windowLine5Rect = (SDL_Rect){SCREEN_WIDTH * 0.5400, SCREEN_HEIGHT * 0.3430, SCREEN_WIDTH * 0.3600, SCREEN_WIDTH * 0.0020};

    // ShipWindowTravelInfo(imageTextures, textTextures, ships);

    SDL_QueryTexture(textTextures[5], NULL, NULL, &textureWidth, &textureHeight);
    category1TitleRect.x = windowRect.x + windowRect.w * 0.03;  // Travel information
    category1TitleRect.y = windowRect.y + windowRect.h * 0.1;
    category1TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category1TitleRect.h = textureHeight * windowRect.w * 0.0005;

    baseDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 0.1240, SCREEN_HEIGHT * 0.2250, SCREEN_WIDTH * 0.0980, SCREEN_WIDTH * 0.0980};

    targetDisplayedRect = (SDL_Rect){SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.2250, SCREEN_WIDTH * 0.0980, SCREEN_WIDTH * 0.0980};

    destRectShip.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    destRectShip.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.13;
    destRectShip.w = windowRect.w * 0.1;
    destRectShip.h = windowRect.w * 0.1;

    narrowRect.x = baseDisplayedRect.x + baseDisplayedRect.w;  
    narrowRect.y = baseDisplayedRect.y + baseDisplayedRect.h * 0.5;
    narrowRect.w = 4;
    narrowRect.h = 15;

    SDL_QueryTexture(textTextures[44], NULL, NULL, &textureWidth, &textureHeight);
    notEnoughFuelRect.x = windowRect.x + windowRect.w * 0.23;
    notEnoughFuelRect.y = windowRect.y + windowRect.h * 0.1;
    notEnoughFuelRect.w = textureWidth * windowRect.w * 0.0005;
    notEnoughFuelRect.h = textureHeight * windowRect.w * 0.0005;

    edgeStopButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.2640, SCREEN_HEIGHT * 0.3500, SCREEN_WIDTH * 0.1300, SCREEN_WIDTH * 0.0220};

    edgeSwapButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.2640, SCREEN_HEIGHT * 0.400, SCREEN_WIDTH * 0.1300, SCREEN_WIDTH * 0.0220};

    SDL_QueryTexture(textTextures[43], NULL, NULL, &textureWidth, &textureHeight);
    stopButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.2940, SCREEN_HEIGHT * 0.3530, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};
    
    SDL_QueryTexture(textTextures[64], NULL, NULL, &textureWidth, &textureHeight);
    stopButtonRect2 = (SDL_Rect){SCREEN_WIDTH * 0.2940, SCREEN_HEIGHT * 0.3530, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};

    stopButtonLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.2720, SCREEN_HEIGHT * 0.3550, SCREEN_WIDTH * 0.0160, SCREEN_WIDTH * 0.0160};

    swapButtonLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.2720, SCREEN_HEIGHT * 0.405, SCREEN_WIDTH * 0.0160, SCREEN_WIDTH * 0.0160};

    SDL_QueryTexture(textTextures[85], NULL, NULL, &textureWidth, &textureHeight);
    swapButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.2940, SCREEN_HEIGHT * 0.4030, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};


    // ShipWindowTankManager(imageTextures, textTextures, ships);

    SDL_QueryTexture(textTextures[8], NULL, NULL, &textureWidth, &textureHeight);
    category2TitleRect.x = windowRect.x + windowRect.w * 0.03;  // Tank manager
    category2TitleRect.y = windowRect.y + windowRect.h * 0.48;
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

    flowInTankRightRect.x = windowRect.x + windowRect.w * 0.3;
    flowInTankRightRect.y = windowRect.y + windowRect.h * 0.6;
    flowInTankRightRect.w = windowRect.w * 0.2;
    flowInTankRightRect.h = windowRect.w * 0.2;

    flowInTankLeftRect.x = windowRect.x + windowRect.w * 0.06;
    flowInTankLeftRect.y = windowRect.y + windowRect.h * 0.6;
    flowInTankLeftRect.w = windowRect.w * 0.2;
    flowInTankLeftRect.h = windowRect.w * 0.2;

    shipRightRect.x = windowRect.x + windowRect.w * 0.26;
    shipRightRect.y = windowRect.y + windowRect.h * 0.68;
    shipRightRect.w = windowRect.w * 0.13;
    shipRightRect.h = windowRect.w * 0.13;

    shipLeftRect.x = windowRect.x + windowRect.w * 0.17;
    shipLeftRect.y = windowRect.y + windowRect.h * 0.68;
    shipLeftRect.w = windowRect.w * 0.13;
    shipLeftRect.h = windowRect.w * 0.13;

    oreToTransfert1Rect.x = windowRect.x + windowRect.w * 0.147;
    oreToTransfert1Rect.y = windowRect.y + windowRect.h * 0.68;
    oreToTransfert1Rect.w = windowRect.w * 0.03;
    oreToTransfert1Rect.h = windowRect.w * 0.03;

    oreToTransfert2Rect.x = windowRect.x + windowRect.w * 0.147;
    oreToTransfert2Rect.y = windowRect.y + windowRect.h * 0.815;
    oreToTransfert2Rect.w = windowRect.w * 0.03;
    oreToTransfert2Rect.h = windowRect.w * 0.03;

    oreToTransfert3Rect.x = windowRect.x + windowRect.w * 0.385;
    oreToTransfert3Rect.y = windowRect.y + windowRect.h * 0.68;
    oreToTransfert3Rect.w = windowRect.w * 0.03;
    oreToTransfert3Rect.h = windowRect.w * 0.03;

    oreToTransfert4Rect.x = windowRect.x + windowRect.w * 0.385;
    oreToTransfert4Rect.y = windowRect.y + windowRect.h * 0.815;
    oreToTransfert4Rect.w = windowRect.w * 0.03;
    oreToTransfert4Rect.h = windowRect.w * 0.03;

    changeTankManagerLeft = (SDL_Rect){SCREEN_WIDTH * 0.2284, SCREEN_HEIGHT * 0.5459, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0190};

    changeTankManagerRight = (SDL_Rect){SCREEN_WIDTH * 0.40, SCREEN_HEIGHT * 0.5459, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0190};

    orePossibility0.x = windowRect.x + windowRect.w * 0.1;
    orePossibility0.y = windowRect.y + windowRect.h * 0.9;
    orePossibility0.w = windowRect.w * 0.05;
    orePossibility0.h = windowRect.w * 0.05;

    orePossibility1.x = windowRect.x + windowRect.w * 0.16;
    orePossibility1.y = windowRect.y + windowRect.h * 0.9;
    orePossibility1.w = windowRect.w * 0.05;
    orePossibility1.h = windowRect.w * 0.05;

    orePossibility2.x = windowRect.x + windowRect.w * 0.22;
    orePossibility2.y = windowRect.y + windowRect.h * 0.9;
    orePossibility2.w = windowRect.w * 0.05;
    orePossibility2.h = windowRect.w * 0.05;

    orePossibility3.x = windowRect.x + windowRect.w * 0.28;
    orePossibility3.y = windowRect.y + windowRect.h * 0.9;
    orePossibility3.w = windowRect.w * 0.05;
    orePossibility3.h = windowRect.w * 0.05;

    orePossibility4.x = windowRect.x + windowRect.w * 0.34;
    orePossibility4.y = windowRect.y + windowRect.h * 0.9;
    orePossibility4.w = windowRect.w * 0.05;
    orePossibility4.h = windowRect.w * 0.05;

    orePossibility5.x = windowRect.x + windowRect.w * 0.4;
    orePossibility5.y = windowRect.y + windowRect.h * 0.9;
    orePossibility5.w = windowRect.w * 0.05;
    orePossibility5.h = windowRect.w * 0.05;

    edgeTankChoosenButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.2240, SCREEN_HEIGHT * 0.540, SCREEN_WIDTH * 0.2000, SCREEN_WIDTH * 0.0240};


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
    shipConditionRect.y = windowRect.y + windowRect.h * 0.14;
    shipConditionRect.w = windowRect.w * 0.11;
    shipConditionRect.h = windowRect.w * 0.11;

    shipConditionSrcRect.x = 0;
    shipConditionSrcRect.y = 0;
    shipConditionSrcRect.w = 64;
    shipConditionSrcRect.h = 64;

    currentLifeShipRect = (SDL_Rect){SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.245, SCREEN_WIDTH * 0.21, SCREEN_WIDTH * 0.013};

    maxLifeShipRect = (SDL_Rect){SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.245, SCREEN_WIDTH * 0.21, SCREEN_WIDTH * 0.013};

    repareShipButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.7000, SCREEN_HEIGHT * 0.279, SCREEN_WIDTH * 0.1150, SCREEN_WIDTH * 0.026};

    SDL_QueryTexture(textTextures[65], NULL, NULL, &textureWidth, &textureHeight);
    repareShipButtonRect2 = (SDL_Rect){SCREEN_WIDTH * 0.74, SCREEN_HEIGHT * 0.285, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};

    logoRepairButtonShipInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.705, SCREEN_HEIGHT * 0.285, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0190};


    // ShipWindowShipModel

    upgradeShipButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.67, SCREEN_HEIGHT * 0.52, SCREEN_WIDTH * 0.1150, SCREEN_WIDTH * 0.026};

    SDL_QueryTexture(textTextures[63], NULL, NULL, &textureWidth, &textureHeight);
    upgradeShipButtonRect2 = (SDL_Rect){SCREEN_WIDTH * 0.705, SCREEN_HEIGHT * 0.525, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};

    logoUpgradeButtonShipInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.6750, SCREEN_HEIGHT * 0.5250, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0190};


    // ShipWindowTankCompo(imageTextures, textTextures);
       
    SDL_QueryTexture(textTextures[14], NULL, NULL, &textureWidth, &textureHeight);
    category5TitleRect.x = windowRect.x + windowRect.w * 0.58;  // Tank composition
    category5TitleRect.y = windowRect.y + windowRect.h * 0.61;
    category5TitleRect.w = textureWidth * windowRect.w * 0.0005;
    category5TitleRect.h = textureHeight * windowRect.w * 0.0005;

    shipFirstCompartmentRect = (SDL_Rect){SCREEN_WIDTH * 0.7890, SCREEN_HEIGHT * 0.7930, SCREEN_WIDTH * 0.0500, SCREEN_WIDTH * 0.0210};

    shipFirstCompartmentLogoRect = (SDL_Rect){SCREEN_WIDTH * 0.76600, SCREEN_HEIGHT * 0.7940, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0200};

    shipFirstCompartmentNumberRect = (SDL_Rect){SCREEN_WIDTH * 0.8070, SCREEN_HEIGHT * 0.7870, SCREEN_WIDTH * 0.0200, SCREEN_WIDTH * 0.0280};

    edgeTankChoosenButtonRect2 = (SDL_Rect){SCREEN_WIDTH * 0.56500, SCREEN_HEIGHT * 0.6400, SCREEN_WIDTH * 0.1600, SCREEN_WIDTH * 0.0200};

    shipRightRect2 = (SDL_Rect){SCREEN_WIDTH * 0.6330, SCREEN_HEIGHT * 0.5700, SCREEN_WIDTH * 0.3500, SCREEN_WIDTH * 0.2700};

    mainInfoTanksEdgeRect = (SDL_Rect){SCREEN_WIDTH * 0.7460, SCREEN_HEIGHT * 0.6410, SCREEN_WIDTH * 0.1300, SCREEN_WIDTH * 0.1350};
       
    SDL_QueryTexture(textTextures[47], NULL, NULL, &textureWidth, &textureHeight);
    mainInfoTanksInfoRect.x = windowRect.x + windowRect.w * 0.855;
    mainInfoTanksInfoRect.y = windowRect.y + windowRect.h * 0.64;
    mainInfoTanksInfoRect.w = textureWidth * windowRect.w * 0.0004;
    mainInfoTanksInfoRect.h = textureHeight * windowRect.w * 0.0004;
       
    SDL_QueryTexture(textTextures[63], NULL, NULL, &textureWidth, &textureHeight);
    upgradeButtonTankRect2 = (SDL_Rect){SCREEN_WIDTH * 0.62, SCREEN_HEIGHT * 0.84, SCREEN_WIDTH * textureWidth * 0.0003, SCREEN_WIDTH * textureHeight * 0.0003};

    upgradeButtonTankRect = (SDL_Rect){SCREEN_WIDTH * 0.58, SCREEN_HEIGHT * 0.835, SCREEN_WIDTH * 0.1150, SCREEN_WIDTH * 0.026};

    logoUpgradeButtonTankInfoRect = (SDL_Rect){SCREEN_WIDTH * 0.585, SCREEN_HEIGHT * 0.84, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0190};

    changeTankManagerLeft2 = (SDL_Rect){SCREEN_WIDTH * 0.5690, SCREEN_HEIGHT * 0.6430, SCREEN_WIDTH * 0.0150, SCREEN_WIDTH * 0.0160};

    changeTankManagerRight2 = (SDL_Rect){SCREEN_WIDTH * 0.705, SCREEN_HEIGHT * 0.6430, SCREEN_WIDTH * 0.0150, SCREEN_WIDTH * 0.0160};

    currentVisualNarrowRect = (SDL_Rect){SCREEN_WIDTH * 0.8400, SCREEN_HEIGHT * 0.7950, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0200};
}

void displayShipWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets) {
    ShipWindowFondations(imageTextures, textTextures);
    ShipWindowTravelInfo(imageTextures, textTextures, ships, planets);
    ShipWindowTankManager(imageTextures, textTextures, ships, planets);
    ShipWindowShipCond(imageTextures, textTextures, ships);
    ShipWindowShipModel(imageTextures, textTextures);
    ShipWindowTankCompo(imageTextures, textTextures, ships);
}

void ShipWindowFondations(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du bg
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &windowRect);

    // Affichage de la croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect);

    // Affichage du titre de la page
    SDL_RenderCopy(renderer, textTextures[32], NULL, &windowTitleRect);

    // Affichages des barres qui separent les informations de la page
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &windowLine1Rect);  // Grande barre horizontale haut
    SDL_RenderFillRect(renderer, &windowLine2Rect);  // Barre horizontale milieu
    SDL_RenderFillRect(renderer, &windowLine3Rect);  // Barre horizontale droite milieu
    SDL_RenderFillRect(renderer, &windowLine4Rect);  // Barre verticale du milieu
    SDL_RenderFillRect(renderer, &windowLine5Rect);  // Barre horizontale droite haut
}

void ShipWindowTravelInfo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets) {
    // Affichage du titre "Travel information"
    SDL_RenderCopy(renderer, textTextures[5], NULL, &category1TitleRect);

    // Affichage des deux planetes
    int idPicture;
    if (ships[getWindowId()].base.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].base.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].base.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &baseDisplayedRect);
    }

    if (ships[getWindowId()].target.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].target.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].target.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &targetDisplayedRect);
    }

    // Affichage du nom des deux planetes
    SDL_RenderCopy(renderer, textTextures[33], NULL, &nameBaseDisplayedRect);
    SDL_RenderCopy(renderer, textTextures[34], NULL, &nameTargetDisplayedRect);

    // Affichage bouton stop 
    if (ships[getWindowId()].state == STOPPED_ON_BASE || ships[getWindowId()].state == STOPPED_ON_TARGET) {  // Bouton redemarrage
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        SDL_RenderFillRect(renderer, &edgeStopButtonRect);
        SDL_RenderCopy(renderer, textTextures[64], NULL, &stopButtonRect2);
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


    // Affichage "OUT OF FUEL!"
    if (ships[getWindowId()].state == OUT_OF_FUEL) {
        SDL_RenderCopy(renderer, textTextures[44], NULL, &notEnoughFuelRect);
    }

    // Affichage du systeme de progression de la fusee dans l'espace
    int dp = targetDisplayedRect.x - baseDisplayedRect.x - baseDisplayedRect.w;  // Distance en pixel entre 2 planetes sur fenetre
    float f;
    
    if (ships[getWindowId()].base.type == SPOT_PLANET && ships[getWindowId()].target.type == SPOT_PLANET) {
        if (ships[getWindowId()].state == MOVING_TO_TARGET || ships[getWindowId()].state == MOVING_TO_TARGET_SOON_STOPPED || 
            ships[getWindowId()].state == WAITING_ON_BASE || ships[getWindowId()].state == WAITING_ON_BASE_SOON_STOPPED) {  
            // Fraction du chemin parcourue
            f = distanceShipPlanet(&ships[getWindowId()], &planets[ships[getWindowId()].base.id_planet])
                / (distancePlanetPlanet(&planets[ships[getWindowId()].target.id_planet], &planets[ships[getWindowId()].base.id_planet]) - planets[ships[getWindowId()].base.id_planet].radius - planets[ships[getWindowId()].target.id_planet].radius);
            f = (f > 1) ? 1 : f;

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

        } else if (ships[getWindowId()].state == MOVING_TO_BASE || ships[getWindowId()].state == MOVING_TO_BASE_SOON_STOPPED || 
                   ships[getWindowId()].state == WAITING_ON_TARGET || ships[getWindowId()].state == WAITING_ON_TARGET_SOON_STOPPED) { 
            // Fraction du chemin parcourue
            f = distanceShipPlanet(&ships[getWindowId()], &planets[ships[getWindowId()].base.id_planet])
                / (distancePlanetPlanet(&planets[ships[getWindowId()].target.id_planet], &planets[ships[getWindowId()].base.id_planet]) - planets[ships[getWindowId()].base.id_planet].radius - planets[ships[getWindowId()].target.id_planet].radius);
            f = (f > 1) ? 1 : f;

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
        } else {
            // Fraction du chemin parcourue
            f = distanceShipPlanet(&ships[getWindowId()], &planets[ships[getWindowId()].base.id_planet])
                / (distancePlanetPlanet(&planets[ships[getWindowId()].target.id_planet], &planets[ships[getWindowId()].base.id_planet]) - planets[ships[getWindowId()].base.id_planet].radius - planets[ships[getWindowId()].target.id_planet].radius);
            f = (f > 1) ? 1 : f;

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
                SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &srcRectShip, &destRectShip, 90, NULL, SDL_FLIP_NONE);
            } else if (ships[getWindowId()].state == STOPPED_ON_TARGET) {
                SDL_RenderCopyEx(renderer, imageTextures[7][ships[getWindowId()].idModel], &srcRectShip, &destRectShip, 270, NULL, SDL_FLIP_NONE);
            } else {
                SDL_RenderCopy(renderer, imageTextures[7][ships[getWindowId()].idModel], &srcRectShip, &destRectShip);
            }
        }
    }
}

void ShipWindowTankManager(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets) {
    // Affichage du titre "Tank manager"
    SDL_RenderCopy(renderer, textTextures[8], NULL, &category2TitleRect);

    // Affichage des deux planetes
    int idPicture;

    if (ships[getWindowId()].base.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].base.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].base.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &baseDisplayedRect2);
    }

    if (ships[getWindowId()].target.type == SPOT_PLANET) {
        idPicture = (planets[ships[getWindowId()].target.id_planet].planetType == SUN) ? 9 : generateRandNb8(currentSeed, ships[getWindowId()].target.id_planet) % 7 + 1;
        SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &targetDisplayedRect2);
    }

    // Affichage du tank en cours de modification ("Tank 1")
    SDL_RenderCopy(renderer, textTextures[15 + currentTankIndex], NULL, &modifyingTankRect);

    // Afficher les fleches pour pouvoir modifier le tank en cours de modification
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &changeTankManagerLeft);
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &changeTankManagerRight, 0, NULL, SDL_FLIP_HORIZONTAL);

    // Affichage bordure du choix de tank
    SDL_DrawEdgeOfRect(edgeTankChoosenButtonRect, 5, BLACK);

    // Affichage du flux de minerais
    SDL_RenderCopy(renderer, imageTextures[5][5], NULL, &flowInTankRightRect);
    SDL_RenderCopyEx(renderer, imageTextures[5][5], NULL, &flowInTankLeftRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

    // Affichage de la fusee
    SDL_RenderCopy(renderer, imageTextures[5][7], &shipConditionSrcRect, &shipRightRect);
    SDL_RenderCopy(renderer, imageTextures[5][7], &shipConditionSrcRect, &shipLeftRect);

    // Affiche les deux lignes separatrices entre les deux fusees
    plotPath((SDL_Point){600, 670}, (SDL_Point){600, 950}, 10, 5, BLACK);
    plotPath((SDL_Point){650, 670}, (SDL_Point){650, 950}, 10, 5, BLACK);

    // Affichage des minerais choisis pour les transferts
    SDL_RenderCopy(renderer, imageTextures[4][ships[getWindowId()].cargo.compartmentsList[currentTankIndex].flowBase_in], NULL, &oreToTransfert1Rect);
    SDL_RenderCopy(renderer, imageTextures[4][ships[getWindowId()].cargo.compartmentsList[currentTankIndex].flowBase_out], NULL, &oreToTransfert2Rect);
    SDL_RenderCopy(renderer, imageTextures[4][ships[getWindowId()].cargo.compartmentsList[currentTankIndex].flowTarget_in], NULL, &oreToTransfert3Rect);
    SDL_RenderCopy(renderer, imageTextures[4][ships[getWindowId()].cargo.compartmentsList[currentTankIndex].flowTarget_out], NULL, &oreToTransfert4Rect);

    // Si modification des minerais choisis pour les transfert, affichage des possibilites
    if (currentOreParameterIndex != -1) {
        SDL_RenderCopy(renderer, imageTextures[4][0], NULL, &orePossibility0);
        SDL_RenderCopy(renderer, imageTextures[4][1], NULL, &orePossibility1);
        SDL_RenderCopy(renderer, imageTextures[4][2], NULL, &orePossibility2);
        SDL_RenderCopy(renderer, imageTextures[4][3], NULL, &orePossibility3);
        SDL_RenderCopy(renderer, imageTextures[4][4], NULL, &orePossibility4);
        SDL_RenderCopy(renderer, imageTextures[4][5], NULL, &orePossibility5);
    }
}

void ShipWindowShipCond(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Ship condition"
    SDL_RenderCopy(renderer, textTextures[11], NULL, &category3TitleRect);

    // Affichage de la fusee
    SDL_RenderCopy(renderer, imageTextures[7][ships[getWindowId()].idModel], &shipConditionSrcRect, &shipConditionRect);

    // Affichage etat de la fusee (fond)
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &maxLifeShipRect);

    // Affichage etat de la fusee (barre actuelle)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    currentLifeShipRect.w = maxLifeShipRect.w * ships[getWindowId()].currentLife / (float)ships[getWindowId()].maxLife;
    SDL_RenderFillRect(renderer, &currentLifeShipRect);

    // Affiche le fond de la barre de vie
    SDL_DrawEdgeOfRect(maxLifeShipRect, 3, BLACK);

    // Affiche le bouton de reparation du vaisseau
    SDL_SetRenderDrawColor(renderer, 255, 232, 207, 255);
    SDL_RenderFillRect(renderer, &repareShipButtonRect);
    SDL_DrawEdgeOfRect(repareShipButtonRect, 3, BLACK);
    SDL_RenderCopy(renderer, textTextures[65], NULL, &repareShipButtonRect2);

    // Affichage du pourcentage de sante de la fusee
    SDL_RenderCopy(renderer, textTextures[36], NULL, &percentShipHealthRect);

    // Afficher le logo du bouton de reparation
    SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &logoRepairButtonShipInfoRect);

}

void ShipWindowShipModel(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Affichage du titre "Ship model"
    SDL_RenderCopy(renderer, textTextures[13], NULL, &category4TitleRect);

    // Affichage de la description de la fusee
    SDL_RenderCopy(renderer, textTextures[37], NULL, &shipInfoRect);
    SDL_RenderCopy(renderer, textTextures[38], NULL, &shipInfoRect2);

    // Affiche le bouton d'amelioration du vaisseau
    SDL_SetRenderDrawColor(renderer, 125, 197, 46, 255);
    SDL_RenderFillRect(renderer, &upgradeShipButtonRect);
    SDL_DrawEdgeOfRect(upgradeShipButtonRect, 3, BLACK);
    SDL_RenderCopy(renderer, textTextures[63], NULL, &upgradeShipButtonRect2);

    // Afficher le logo du bouton d'amelioration
    SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &logoUpgradeButtonShipInfoRect);

}

void ShipWindowTankCompo(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships) {
    // Affichage du titre "Tank composition"
    SDL_RenderCopy(renderer, textTextures[14], NULL, &category5TitleRect);

    // Affiche la fusee en arriere plan a droite (reservoir par dessus)
    SDL_RenderCopy(renderer, imageTextures[5][7], &shipConditionSrcRect, &shipRightRect2);

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

    // Affiche une fleche pour annoncer le tank en cours de modification
    SDL_Rect currentVisualNarrowRectTempo = currentVisualNarrowRect;
    for (int i = 0; i < currentTankIndex; i++) {
        currentVisualNarrowRectTempo.y -= GapBetweenTanks * shipFirstCompartmentRect.h;
    }
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &currentVisualNarrowRectTempo);

    // Affichage fleches pour changer le tank    // Afficher les fleches pour pouvoir modifier le tank en cours de modification
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &changeTankManagerLeft2);
    SDL_RenderCopyEx(renderer, imageTextures[2][2], NULL, &changeTankManagerRight2, 0, NULL, SDL_FLIP_HORIZONTAL);

    // Affichage du tank en cours de modification ("Tank 1")
    SDL_RenderCopy(renderer, textTextures[15 + currentTankIndex], NULL, &infoPerTankRect);

    // Affichage bordure du choix de tank
    SDL_DrawEdgeOfRect(edgeTankChoosenButtonRect2, 5, BLACK);

    // Affiche la description du tank 
    SDL_RenderCopy(renderer, textTextures[35], NULL, &tankInfoRect);

    // Affiche le bouton d'amelioration du tank
    SDL_SetRenderDrawColor(renderer, 125, 197, 46, 255);
    SDL_RenderFillRect(renderer, &upgradeButtonTankRect);
    SDL_DrawEdgeOfRect(upgradeButtonTankRect, 3, BLACK);
    SDL_RenderCopy(renderer, textTextures[63], NULL, &upgradeButtonTankRect2);

    // Afficher le logo du bouton d'amelioration du tank
    SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &logoUpgradeButtonTankInfoRect);
}

void shipWindowGestion(SDL_Texture **textTextures, TTF_Font **fonts, Mix_Chunk **sounds, Ship *ship, Planet *planets, SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
        Mix_PlayChannel(1, sounds[10], 0);
        setWindowType(NO_WINDOW);
    }

    // Choix du tank
    else if ((SDL_PointInRect(&mouse, &changeTankManagerLeft) || SDL_PointInRect(&mouse, &changeTankManagerLeft2))) {
        currentTankIndex --;
        if (currentTankIndex < 0) {
            currentTankIndex += ship->cargo.compartmentsNumber;
        }
        initShipWindow(textTextures, fonts, ship);
        Mix_PlayChannel(1, sounds[7], 0);

    } 
    else if ((SDL_PointInRect(&mouse, &changeTankManagerRight) || SDL_PointInRect(&mouse, &changeTankManagerRight2))) {
        currentTankIndex ++;
        if (currentTankIndex > ship->cargo.compartmentsNumber - 1) {
            currentTankIndex -= ship->cargo.compartmentsNumber;
        }
        initShipWindow(textTextures, fonts, ship);
        Mix_PlayChannel(1, sounds[7], 0);
    }

    // Ouverture de la fenetre de la planete (depuis la fenetre d'info. de la fusee)
    else if (SDL_PointInRect(&mouse, &baseDisplayedRect) || SDL_PointInRect(&mouse, &baseDisplayedRect2)) {
        if (ship->base.type == SPOT_PLANET) {
            setWindowType(PLANET_WINDOW);
            setWindowId(ship->base.id_planet);
            initPlanetWindow(textTextures, fonts, planets);
            setCameraLastObjectSelected(ship->base.id_planet);
            setCameraMode(FOLLOW_PLANET);
        }
    }
    else if (SDL_PointInRect(&mouse, &targetDisplayedRect) || SDL_PointInRect(&mouse, &targetDisplayedRect2)) {
        if (ship->target.type == SPOT_PLANET) {
            setWindowType(PLANET_WINDOW);
            setWindowId(ship->target.id_planet);
            initPlanetWindow(textTextures, fonts, planets);
            setCameraLastObjectSelected(ship->target.id_planet);
            setCameraMode(FOLLOW_PLANET);
        }
    }

    // Reparation de la fusee
    else if (SDL_PointInRect(&mouse, &repareShipButtonRect)) {
        ship->currentLife += 0.1 * ship->maxLife;

        if (ship->currentLife > ship->maxLife) {
            ship->currentLife = ship->maxLife;
        }
        initShipWindow(textTextures, fonts, ship);
        Mix_PlayChannel(1, sounds[6], 0);
    }

    // Amelioration de la fusee
    else if (SDL_PointInRect(&mouse, &upgradeShipButtonRect)) {
        ship->level += 1;
        initShipWindow(textTextures, fonts, ship);
        Mix_PlayChannel(1, sounds[5], 0);
    }

    // Amelioration d'un tank de la fusee
    else if (SDL_PointInRect(&mouse, &upgradeButtonTankRect)) {
        ship->cargo.compartmentsList[currentTankIndex].level += 1;
        initShipWindow(textTextures, fonts, ship);
        Mix_PlayChannel(1, sounds[5], 0);
    }

    // Systeme d'arret d'urgence de la fusee
    else if (SDL_PointInRect(&mouse, &edgeStopButtonRect)) {
        // La fusee s'arretera au prochain arret (apres avoir transferre ses minerais)
        if (ship->state == MOVING_TO_BASE) {
            Mix_PlayChannel(1, sounds[8], 0);
            ship->state = MOVING_TO_BASE_SOON_STOPPED;
            initShipWindow(textTextures, fonts, ship);
        } else if(ship->state == MOVING_TO_TARGET) {
            Mix_PlayChannel(1, sounds[8], 0);
            ship->state = MOVING_TO_TARGET_SOON_STOPPED;
            initShipWindow(textTextures, fonts, ship);
        } else if (ship->state == MOVING_TO_BASE_SOON_STOPPED) {
            Mix_PlayChannel(1, sounds[8], 0);
            ship->state = MOVING_TO_BASE;
            initShipWindow(textTextures, fonts, ship);
        } else if(ship->state == MOVING_TO_TARGET_SOON_STOPPED) {
            Mix_PlayChannel(1, sounds[8], 0);
            ship->state = MOVING_TO_TARGET;
            initShipWindow(textTextures, fonts, ship);
        }

        // Redemarrage des moteur (apres avoir transfere ses minerais)
        else if (ship->state == STOPPED_ON_BASE) {
            Mix_PlayChannel(1, sounds[8], 0);
            if (ship->base.type == SPOT_PLANET) {
                ship->state = WAITING_ON_BASE;  // Pour recuperer les ressources avant de partir de la planete
            } else {
                ship->state = MOVING_TO_TARGET;
            }
            initShipWindow(textTextures, fonts, ship);
        } else if (ship->state == STOPPED_ON_TARGET) {
            Mix_PlayChannel(1, sounds[8], 0);
            if (ship->base.type == SPOT_PLANET) {
                ship->state = WAITING_ON_TARGET;  // Pour recuperer les ressources avant de partir de la planete
            } else {
                ship->state = MOVING_TO_BASE;
            }
            initShipWindow(textTextures, fonts, ship);
        }
    }

    // Choix de la case a modifier
    else if (SDL_PointInRect(&mouse, &oreToTransfert1Rect)) {
        currentOreParameterIndex = 0;
    } 
    else if (SDL_PointInRect(&mouse, &oreToTransfert2Rect)) {
        currentOreParameterIndex = 1;
    } 
    else if (SDL_PointInRect(&mouse, &oreToTransfert3Rect)) {
        currentOreParameterIndex = 2;
    } 
    else if (SDL_PointInRect(&mouse, &oreToTransfert4Rect)) {
        currentOreParameterIndex = 3;
    } 

    // Echange de la destination si la fusee est en vol
    else if (SDL_PointInRect(&mouse, &swapButtonRect)) {
        if (ship->state == MOVING_TO_BASE) {
            ship->state = MOVING_TO_TARGET;
        } else if (ship->state == MOVING_TO_TARGET) {
            ship->state = MOVING_TO_BASE;
        } else if (ship->state == MOVING_TO_BASE_SOON_STOPPED) {
            ship->state = MOVING_TO_TARGET_SOON_STOPPED;
        } else if (ship->state == MOVING_TO_TARGET_SOON_STOPPED) {
            ship->state = MOVING_TO_BASE_SOON_STOPPED;
        }
    }

    // Choix du nouveau minerai transfere pour le rectangle prealablement choisi
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility0)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 0;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 0;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 0;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 0;
                break;
            default:
                break;

        }
    }  
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility1)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 1;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 1;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 1;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 1;
                break;
            default:
                break;

        }
    }  
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility2)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 2;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 2;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 2;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 2;
                break;
            default:
                break;

        }
    }  
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility3)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 3;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 3;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 3;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 3;
                break;
            default:
                break;

        }
    }
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility4)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 4;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 4;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 4;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 4;
                break;
            default:
                break;

        }
    }   
    else if (currentOreParameterIndex != -1 && SDL_PointInRect(&mouse, &orePossibility5)) {
        switch (currentOreParameterIndex) {
            case 0:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_in = 5;
                break;
            case 1:
                ship->cargo.compartmentsList[currentTankIndex].flowBase_out = 5;
                break;
            case 2:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_in = 5;
                break;
            case 3:
                ship->cargo.compartmentsList[currentTankIndex].flowTarget_out = 5;
                break;
            default:
                break;

        }
    }     


    else {
        currentOreParameterIndex = -1;
    }
    
}
