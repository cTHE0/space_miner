#include "pause_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "renderer.h"
#include "place.h"
#include "tools.h"
#include "assets_gestion.h"
#include "landing_page.h"
#include "window.h"
#include "planet.h"
#include "ship.h"
#include "tile.h"


static SDL_Rect windowRect,
                backToGameButtonRect,
                backToGameButtonBgRect,
                advancementsButtonRect,
                advancementsButtonBgRect,
                settingsButtonRect,
                settingsButtonBgRect,
                saveAndQuitButtonRect,
                saveAndQuitButtonBgRect,
                reportBugsButtonRect,
                reportBugsButtonBgRect,
                statisticsButtonRect,
                statisticsButtonBgRect,
                meteor1Rect,
                meteor2Rect,
                meteor3Rect;


/*
void initPauseWindow(SDL_Texture **textTextures, TTF_Font **fonts) {
    int textureWidth, textureHeight;

}
*/

void initPauseWindowRects(SDL_Texture **textTextures) {
    int textureWidth, textureHeight;  // Permet de garder les proportions des textes

    windowRect = (SDL_Rect){(1 - 0.45) * SCREEN_WIDTH / 2., (1 - 0.6) * SCREEN_HEIGHT / 2., SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.6};

    SDL_QueryTexture(textTextures[79], NULL, NULL, &textureWidth, &textureHeight);
    backToGameButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.27, textureWidth * SCREEN_HEIGHT * 0.0010, textureHeight * SCREEN_HEIGHT * 0.0010};

    SDL_QueryTexture(textTextures[80], NULL, NULL, &textureWidth, &textureHeight);
    advancementsButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.31, SCREEN_HEIGHT * 0.425, textureWidth * SCREEN_HEIGHT * 0.001, textureHeight * SCREEN_HEIGHT * 0.001};

    SDL_QueryTexture(textTextures[81], NULL, NULL, &textureWidth, &textureHeight);
    settingsButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.345, SCREEN_HEIGHT * 0.545, textureWidth * SCREEN_HEIGHT * 0.001, textureHeight * SCREEN_HEIGHT * 0.001};

    SDL_QueryTexture(textTextures[82], NULL, NULL, &textureWidth, &textureHeight);
    saveAndQuitButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.685, textureWidth * SCREEN_HEIGHT * 0.001, textureHeight * SCREEN_HEIGHT * 0.001};

    SDL_QueryTexture(textTextures[83], NULL, NULL, &textureWidth, &textureHeight);
    reportBugsButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.535, SCREEN_HEIGHT * 0.545, textureWidth * SCREEN_HEIGHT * 0.001, textureHeight * SCREEN_HEIGHT * 0.001};

    SDL_QueryTexture(textTextures[84], NULL, NULL, &textureWidth, &textureHeight);
    statisticsButtonRect = (SDL_Rect){SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.425, textureWidth * SCREEN_HEIGHT * 0.001, textureHeight * SCREEN_HEIGHT * 0.001};

    backToGameButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.3900, SCREEN_HEIGHT * 0.2550, SCREEN_WIDTH * 0.2220, SCREEN_WIDTH * 0.0500};
    advancementsButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.2900, SCREEN_HEIGHT * 0.4100, SCREEN_WIDTH * 0.2000, SCREEN_WIDTH * 0.0500};
    settingsButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.2900, SCREEN_HEIGHT * 0.5300, SCREEN_WIDTH * 0.2000, SCREEN_WIDTH * 0.0500};
    statisticsButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.500, SCREEN_HEIGHT * 0.4100, SCREEN_WIDTH * 0.200, SCREEN_WIDTH * 0.0500};
    reportBugsButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.500, SCREEN_HEIGHT * 0.5300, SCREEN_WIDTH * 0.200, SCREEN_WIDTH * 0.0500};
    saveAndQuitButtonBgRect = (SDL_Rect){SCREEN_WIDTH * 0.3900, SCREEN_HEIGHT * 0.67, SCREEN_WIDTH * 0.2220, SCREEN_WIDTH * 0.0500};
    meteor1Rect = (SDL_Rect){SCREEN_WIDTH * 0.3000, SCREEN_HEIGHT * 0.6200, SCREEN_WIDTH * 0.0700, SCREEN_WIDTH * 0.0700};
    meteor2Rect = (SDL_Rect){SCREEN_WIDTH * 0.3600, SCREEN_HEIGHT * 0.3300, SCREEN_WIDTH * 0.0500, SCREEN_WIDTH * 0.0500};
    meteor3Rect = (SDL_Rect){SCREEN_WIDTH * 0.6700, SCREEN_HEIGHT * 0.5500, SCREEN_WIDTH * 0.0500, SCREEN_WIDTH * 0.0500};
}

void displayPauseWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    pauseWindowFondations();
    pauseWindowAesthetic(imageTextures);
    pauseWindowButtons(textTextures);
}

void pauseWindowFondations(void) {
    // Affichage du fond de la page
    SDL_SetRenderDrawColor(renderer, 18, 52, 73, 255);
    SDL_RenderFillRect(renderer, &windowRect);

    // Affichage des bords de la page
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_DrawEdgeOfRect(windowRect, 3, DARK_RED);
}

void pauseWindowAesthetic(SDL_Texture ***imageTextures) {
    SDL_RenderCopy(renderer, imageTextures[0][0], NULL, &meteor1Rect);
    SDL_RenderCopy(renderer, imageTextures[0][1], NULL, &meteor2Rect);
    SDL_RenderCopy(renderer, imageTextures[0][1], NULL, &meteor3Rect);
}

void pauseWindowButtons(SDL_Texture **textTextures) {
    // Affichage du bg des boutons
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &backToGameButtonBgRect);
    SDL_RenderFillRect(renderer, &advancementsButtonBgRect);
    SDL_RenderFillRect(renderer, &settingsButtonBgRect);
    SDL_RenderFillRect(renderer, &statisticsButtonBgRect);
    SDL_RenderFillRect(renderer, &reportBugsButtonBgRect);
    SDL_RenderFillRect(renderer, &saveAndQuitButtonBgRect);

    // Affichage du bord du bg des boutons
    SDL_DrawEdgeOfRect(backToGameButtonBgRect, 3, BLACK);
    SDL_DrawEdgeOfRect(advancementsButtonBgRect, 3, BLACK);
    SDL_DrawEdgeOfRect(settingsButtonBgRect, 3, BLACK);
    SDL_DrawEdgeOfRect(statisticsButtonBgRect, 3, BLACK);
    SDL_DrawEdgeOfRect(reportBugsButtonBgRect, 3, BLACK);
    SDL_DrawEdgeOfRect(saveAndQuitButtonBgRect, 3, BLACK);

    // Affichage du texte sur les boutons
    SDL_RenderCopy(renderer, textTextures[79], NULL, &backToGameButtonRect);
    SDL_RenderCopy(renderer, textTextures[80], NULL, &advancementsButtonRect);
    SDL_RenderCopy(renderer, textTextures[81], NULL, &settingsButtonRect);
    SDL_RenderCopy(renderer, textTextures[82], NULL, &saveAndQuitButtonRect);
    SDL_RenderCopy(renderer, textTextures[83], NULL, &reportBugsButtonRect);
    SDL_RenderCopy(renderer, textTextures[84], NULL, &statisticsButtonRect);
}

void pauseWindowGestion(Mix_Chunk **sounds, GameState *gameState, SDL_Point mouse, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    // Gestion du bouton clique
    if (SDL_PointInRect(&mouse, &backToGameButtonBgRect)) {
        setWindowType(NO_WINDOW);
        Mix_PlayChannel(1, sounds[7], 0);
    } else if (SDL_PointInRect(&mouse, &saveAndQuitButtonBgRect)) {
        setWindowType(NO_WINDOW);
        saveGame(ships, shipCount, planets, planetCount);
        *gameState = QUIT;
        Mix_PlayChannel(1, sounds[7], 0);
    }
}

void saveGame(Ship *ships, int shipCount, Planet *planets, int planetCount) {
    FILE *backup = fopen(NAME_BACKUP, "wb");  // Ouvre le fichier en mode binaire
    if (!backup) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }

    // Sauvegarder le nombre de fusees/planetes au début du fichier
    fwrite(&shipCount, sizeof(int), 1, backup);
    fwrite(&planetCount, sizeof(int), 1, backup);

    // Sauvegarder chaque fusee/planete/tuile dans le fichier
    fwrite(ships, sizeof(Ship), shipCount, backup);
    fwrite(planets, sizeof(Planet), planetCount, backup);
    fwrite(*getTilesMatrix(), sizeof(uint8_t), *getByteCount(), backup);

    // Fermeture du fichier de sauvegarde
    fclose(backup);
}
