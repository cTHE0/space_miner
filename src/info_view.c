#include <SDL2/SDL.h>

#include "info_view.h"
#include "assets_gestion.h"
#include "place.h"
#include "renderer.h"
#include "config.h"
#include "text.h"
#include "ore.h"
#include "window.h"

static SDL_Rect upBarRect, 
                upBarValuesRect,
                rightBarRect, 
                arrowRightBarRect,
                firstIconUpBarRect,
                settingsRect,
                firstValueUpBarRect;

SDL_Rect getSideBarButtonRect() {
    return rightBarRect;
}

SDL_Rect getSettingsIconRect(void) {
    return settingsRect;
}

void initInfoViewRects(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;

    upBarRect = (SDL_Rect){SCREEN_WIDTH * -0.0000, SCREEN_HEIGHT * -0.0450, SCREEN_WIDTH * 1.0000, SCREEN_WIDTH * 0.0500};

    upBarValuesRect = (SDL_Rect){SCREEN_WIDTH * 0.0270, SCREEN_HEIGHT * -0.0050, SCREEN_WIDTH * 0.0080, SCREEN_WIDTH * 0.0310};

    rightBarRect = (SDL_Rect){SCREEN_WIDTH * 0.9820, SCREEN_HEIGHT * 0.4250, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0860};

    arrowRightBarRect = (SDL_Rect){SCREEN_WIDTH * 0.9840, SCREEN_HEIGHT * 0.4890, SCREEN_WIDTH * 0.0150, SCREEN_WIDTH * 0.0140};

    firstIconUpBarRect = (SDL_Rect){SCREEN_WIDTH * 0.005, SCREEN_HEIGHT * 0.0070, SCREEN_WIDTH * 0.0170, SCREEN_WIDTH * 0.0180};

    settingsRect = (SDL_Rect){SCREEN_WIDTH * 0.9540, SCREEN_HEIGHT * 0.0040, SCREEN_WIDTH * 0.0210, SCREEN_WIDTH * 0.0200};

    SDL_QueryTexture(textTextures[24], NULL, NULL, &textureWidth, &textureHeight);
    firstValueUpBarRect = (SDL_Rect){SCREEN_WIDTH * 0.03, SCREEN_HEIGHT * -0.005, textureWidth * SCREEN_WIDTH * 0.0005, textureHeight * SCREEN_WIDTH * 0.0005};
}

void displayInfoView(SDL_Texture ***imageTextures) {
    static int coefGapBetweenOres = 6;

    // Affichage du fond de la barre en haut de l'ecran
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 170);  // gris, 50% transparent
    SDL_RenderFillRect(renderer, &upBarRect);

    // Affichage des icons et des valeurs
    SDL_Rect currentIconRect = firstIconUpBarRect;
    SDL_Rect currentvalueRect = firstValueUpBarRect;
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        // Affichage des logos
        currentIconRect.x = firstIconUpBarRect.x + coefGapBetweenOres * firstIconUpBarRect.w * i;
        SDL_RenderCopy(renderer, imageTextures[4][i], NULL, &currentIconRect);

        // Affichage des valeurs
        currentvalueRect.x = firstValueUpBarRect.x + coefGapBetweenOres * firstIconUpBarRect.w * i;
        renderNumber(getTotalOreWithInt(i), currentvalueRect);
    }

    // Afficher la barre a droite
    if (getWindowType() == NO_WINDOW || getWindowType() == BASIC_SHIP_WINDOW) {
        SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &settingsRect);
        SDL_RenderFillRect(renderer, &rightBarRect);
        SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &arrowRightBarRect);
    }
}