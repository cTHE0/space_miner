#include <SDL2/SDL.h>

#include "info_view.h"
#include "assets_gestion.h"
#include "place.h"
#include "renderer.h"
#include "config.h"
#include "text.h"
#include "ore.h"
#include "window.h"
#include "meta.h"

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

void displayInfoView(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
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

    // Affichage des credits et du score (cote droit de la barre du haut)
    int tw, th;
    SDL_QueryTexture(textTextures[95], NULL, NULL, &tw, &th);
    SDL_Rect creditsLabelRect = {SCREEN_WIDTH * 0.62, SCREEN_HEIGHT * 0.004, tw * SCREEN_HEIGHT * 0.0007, th * SCREEN_HEIGHT * 0.0007};
    SDL_RenderCopy(renderer, textTextures[95], NULL, &creditsLabelRect);
    SDL_Rect creditsValueRect = {SCREEN_WIDTH * 0.685, SCREEN_HEIGHT * -0.005, SCREEN_WIDTH * 0.009, SCREEN_WIDTH * 0.024};
    renderNumber((int)getMeta()->credits, creditsValueRect);

    SDL_QueryTexture(textTextures[96], NULL, NULL, &tw, &th);
    SDL_Rect scoreLabelRect = {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT * 0.004, tw * SCREEN_HEIGHT * 0.0007, th * SCREEN_HEIGHT * 0.0007};
    SDL_RenderCopy(renderer, textTextures[96], NULL, &scoreLabelRect);
    SDL_Rect scoreValueRect = {SCREEN_WIDTH * 0.83, SCREEN_HEIGHT * -0.005, SCREEN_WIDTH * 0.009, SCREEN_WIDTH * 0.024};
    renderNumber((int)getMeta()->score, scoreValueRect);

    // Afficher la barre a droite
    if (getWindowType() == NO_WINDOW || getWindowType() == BASIC_SHIP_WINDOW) {
        SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &settingsRect);
        SDL_RenderFillRect(renderer, &rightBarRect);
        SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &arrowRightBarRect);
    }
}