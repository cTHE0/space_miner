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


// Declaration des rectangles et variables propres a la fenetre d'informations des fusees
static SDL_Rect windowRect,
                WindowCrossRect,
                planetDisplayedRect,
                planetFirstResourceRect,
                planetFirstResourceLogoRect,
                windowLine1Rect,
                windowTitleRect;


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
    
    sprintf(newText.text, "%s %s", type_planet, name_planet);
    newText.color = BLACK;
    newText.font = fonts[0];

    updateTextTexture(&textTextures[30], newText);

    SDL_QueryTexture(textTextures[30], NULL, NULL, &textureWidth, &textureHeight);
    windowTitleRect.x = windowRect.x + windowRect.w * 0.01;  
    windowTitleRect.y = windowRect.y + windowRect.h * 0.007;
    windowTitleRect.w = textureWidth * windowRect.w * 0.0005;
    windowTitleRect.h = textureHeight * windowRect.w * 0.0005;
}

void initRectPlanetWindow(SDL_Texture **textTextures) {
    //int textureWidth, textureHeight;  // Permet de garder les proportions des textes

    windowRect.x = (1 - 0.8) * SCREEN_WIDTH / 2.;
    windowRect.y = (1 - 0.8) * SCREEN_HEIGHT / 2.;
    windowRect.w = SCREEN_WIDTH * 0.8;
    windowRect.h = SCREEN_HEIGHT * 0.8;

    windowLine1Rect.x = windowRect.x;  // Grande barre horizontale haut
    windowLine1Rect.y = windowRect.y + windowRect.h * 0.07;
    windowLine1Rect.w = windowRect.w;
    windowLine1Rect.h = 3;

    WindowCrossRect.x = windowRect.x + windowRect.w * 0.97;
    WindowCrossRect.y = windowRect.y + windowRect.h * 0.025;
    WindowCrossRect.w = windowRect.w * 0.015;
    WindowCrossRect.h = windowRect.w * 0.015;
    
    planetDisplayedRect.x = SCREEN_WIDTH / 8.;
    planetDisplayedRect.y = SCREEN_WIDTH / 3.;
    planetDisplayedRect.w = 300;
    planetDisplayedRect.h = 300;

    planetFirstResourceRect.x = windowRect.x + windowRect.w * 0.85;
    planetFirstResourceRect.y = windowRect.y + windowRect.h * 0.88;
    planetFirstResourceRect.w = windowRect.w * 0.07;
    planetFirstResourceRect.h = windowRect.h * 0.07;

    planetFirstResourceLogoRect.x = windowRect.x + windowRect.w * 0.93;
    planetFirstResourceLogoRect.y = windowRect.y + windowRect.h * 0.89;
    planetFirstResourceLogoRect.w = windowRect.w * 0.04;
    planetFirstResourceLogoRect.h = windowRect.w * 0.04;
}


void displayPlanetWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Planet *planets, int planetCount) {
    // Arriere plan de la fenetre d'informations
    SDL_RenderCopy(renderer, imageTextures[5][3], NULL, &windowRect);

    // Croix pour fermer la fenetre
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &WindowCrossRect); 

    // Barres pour structurer la page 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &windowLine1Rect);

    // Planete decrite dans cette fenetre
    int idPicture = (planets[getWindowId()].planetType == SUN) ? 9 : generateRandNb8(currentSeed, getWindowId()) % 8 + 1;
    SDL_RenderCopy(renderer, imageTextures[6][idPicture], NULL, &planetDisplayedRect);  

    // Affichage des stocks de ressources, l'un apres l'autre
    SDL_Rect currentOreRect = planetFirstResourceRect;
    SDL_Rect currentLogoRect = planetFirstResourceLogoRect;
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
    }

    // Affichage du titre de la page
    SDL_RenderCopy(renderer, textTextures[30], NULL, &windowTitleRect);
}

void planetWindowGestion(SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &WindowCrossRect) || !clickOnWindow(mouse)) {
        setWindowType(NO_WINDOW);
    }
}
