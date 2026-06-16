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
#include "settings.h"
#include "config.h"
#include "meta.h"
#include "text.h"
#include <sys/stat.h>


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
    if (SDL_PointInRect(&mouse, &settingsButtonBgRect)) {
        setWindowType(SETTINGS_WINDOW);
        Mix_PlayChannel(1, sounds[7], 0);
    } else if (SDL_PointInRect(&mouse, &backToGameButtonBgRect) || !SDL_PointInRect(&mouse, &windowRect)) {
        setWindowType(NO_WINDOW);
        Mix_PlayChannel(1, sounds[7], 0);
    } else if (SDL_PointInRect(&mouse, &saveAndQuitButtonBgRect)) {
        setWindowType(NO_WINDOW);
        saveGame(ships, shipCount, planets, planetCount);
        *gameState = QUIT;
        Mix_PlayChannel(1, sounds[7], 0);
    }
}

/* ----------------------- Fenetre de reglages (audio) ----------------------- */

static SDL_Rect settingsPanelRect,
                settingsTitleRect,
                musicToggleRect,
                sfxToggleRect,
                muteToggleRect;


void initSettingsWindowRects(void) {
    settingsPanelRect = (SDL_Rect){SCREEN_WIDTH * 0.30, SCREEN_HEIGHT * 0.25, SCREEN_WIDTH * 0.40, SCREEN_HEIGHT * 0.50};

    // Boutons d'activation (a droite de chaque ligne)
    int toggleW = settingsPanelRect.w * 0.28;
    int toggleH = settingsPanelRect.h * 0.12;
    int toggleX = settingsPanelRect.x + settingsPanelRect.w * 0.60;

    musicToggleRect = (SDL_Rect){toggleX, settingsPanelRect.y + settingsPanelRect.h * 0.30, toggleW, toggleH};
    sfxToggleRect   = (SDL_Rect){toggleX, settingsPanelRect.y + settingsPanelRect.h * 0.50, toggleW, toggleH};
    muteToggleRect  = (SDL_Rect){toggleX, settingsPanelRect.y + settingsPanelRect.h * 0.70, toggleW, toggleH};

    settingsTitleRect = (SDL_Rect){0, 0, 0, 0};  // calcule a l'affichage selon la texture
}

static void drawSettingsLabel(SDL_Texture *textTexture, int rowY) {
    int tw, th;
    SDL_QueryTexture(textTexture, NULL, NULL, &tw, &th);
    SDL_Rect r = {settingsPanelRect.x + settingsPanelRect.w * 0.08, rowY, tw * SCREEN_HEIGHT * 0.0006, th * SCREEN_HEIGHT * 0.0006};
    SDL_RenderCopy(renderer, textTexture, NULL, &r);
}

static void drawToggle(SDL_Texture **textTextures, SDL_Rect rect, int on) {
    SDL_SetRenderDrawColor(renderer, on ? 59 : 90, on ? 120 : 40, on ? 40 : 40, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_DrawEdgeOfRect(rect, 3, BLACK);

    SDL_Texture *label = on ? textTextures[93] : textTextures[94];  // "ON" / "OFF"
    int tw, th;
    SDL_QueryTexture(label, NULL, NULL, &tw, &th);
    SDL_Rect lr = {rect.x + rect.w / 2 - tw * SCREEN_HEIGHT * 0.0006 / 2,
                   rect.y + rect.h / 2 - th * SCREEN_HEIGHT * 0.0006 / 2,
                   tw * SCREEN_HEIGHT * 0.0006, th * SCREEN_HEIGHT * 0.0006};
    SDL_RenderCopy(renderer, label, NULL, &lr);
}

void displaySettingsWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    (void)imageTextures;

    // Fond de la fenetre
    SDL_SetRenderDrawColor(renderer, 18, 52, 73, 255);
    SDL_RenderFillRect(renderer, &settingsPanelRect);
    SDL_DrawEdgeOfRect(settingsPanelRect, 3, WHITE);

    // Titre "Audio settings"
    int tw, th;
    SDL_QueryTexture(textTextures[89], NULL, NULL, &tw, &th);
    settingsTitleRect = (SDL_Rect){settingsPanelRect.x + settingsPanelRect.w / 2 - tw * SCREEN_HEIGHT * 0.0009 / 2,
                                   settingsPanelRect.y + settingsPanelRect.h * 0.08,
                                   tw * SCREEN_HEIGHT * 0.0009, th * SCREEN_HEIGHT * 0.0009};
    SDL_RenderCopy(renderer, textTextures[89], NULL, &settingsTitleRect);

    // Lignes : libelles + boutons
    drawSettingsLabel(textTextures[90], musicToggleRect.y + musicToggleRect.h * 0.15);  // Music
    drawSettingsLabel(textTextures[91], sfxToggleRect.y + sfxToggleRect.h * 0.15);       // Sound effects
    drawSettingsLabel(textTextures[92], muteToggleRect.y + muteToggleRect.h * 0.15);     // Mute all

    drawToggle(textTextures, musicToggleRect, isMusicOn() && !isMuted());
    drawToggle(textTextures, sfxToggleRect, isSfxOn() && !isMuted());
    drawToggle(textTextures, muteToggleRect, isMuted());
}

int settingsWindowGestion(Mix_Chunk **sounds, SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &musicToggleRect)) {
        toggleMusic(sounds);
        Mix_PlayChannel(1, sounds[7], 0);
    } else if (SDL_PointInRect(&mouse, &sfxToggleRect)) {
        toggleSfx();
        Mix_PlayChannel(1, sounds[7], 0);
    } else if (SDL_PointInRect(&mouse, &muteToggleRect)) {
        toggleMute(sounds);
    } else if (!SDL_PointInRect(&mouse, &settingsPanelRect)) {
        setWindowType(NO_WINDOW);  // Clic en dehors : fermeture
        Mix_PlayChannel(1, sounds[7], 0);
        return 1;
    }
    return 0;
}

/* ----------------------- Ecran de fin de partie ----------------------- */

static SDL_Rect gameOverReturnRect;

static void drawCenteredText(SDL_Texture *tex, int cx, int y, int h) {
    if (!tex) return;
    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    int w = (int)(tw * (h / (float)th));
    SDL_Rect r = {cx - w / 2, y, w, h};
    SDL_RenderCopy(renderer, tex, NULL, &r);
}

void displayGameOverWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    (void)imageTextures;
    Meta *m = getMeta();

    // Voile sombre sur tout l'ecran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, NULL);

    SDL_Rect panel = {SCREEN_WIDTH * 0.30, SCREEN_HEIGHT * 0.20, SCREEN_WIDTH * 0.40, SCREEN_HEIGHT * 0.60};
    SDL_SetRenderDrawColor(renderer, 18, 52, 73, 255);
    SDL_RenderFillRect(renderer, &panel);
    SDL_DrawEdgeOfRect(panel, 4, RED);

    int cx = panel.x + panel.w / 2;

    // Titre
    drawCenteredText(textTextures[110], cx, panel.y + panel.h * 0.06, panel.h * 0.13);

    // Score final
    drawCenteredText(textTextures[111], cx, panel.y + panel.h * 0.30, panel.h * 0.06);
    SDL_Rect scoreRect = {cx - panel.w * 0.05, panel.y + panel.h * 0.38, panel.w * 0.018, panel.h * 0.08};
    renderNumber((int)m->score, scoreRect);

    // Statistiques
    drawCenteredText(textTextures[112], cx - panel.w * 0.10, panel.y + panel.h * 0.55, panel.h * 0.045);
    SDL_Rect pkRect = {cx + panel.w * 0.22, panel.y + panel.h * 0.55, panel.w * 0.013, panel.h * 0.05};
    renderNumber(m->piratesKilled, pkRect);

    drawCenteredText(textTextures[113], cx - panel.w * 0.10, panel.y + panel.h * 0.64, panel.h * 0.045);
    SDL_Rect mmRect = {cx + panel.w * 0.22, panel.y + panel.h * 0.64, panel.w * 0.013, panel.h * 0.05};
    renderNumber((int)m->mineralsMined, mmRect);

    // Bouton retour
    gameOverReturnRect = (SDL_Rect){panel.x + panel.w * 0.25, panel.y + panel.h * 0.80, panel.w * 0.50, panel.h * 0.12};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &gameOverReturnRect);
    SDL_DrawEdgeOfRect(gameOverReturnRect, 3, BLACK);
    drawCenteredText(textTextures[114], cx, gameOverReturnRect.y + gameOverReturnRect.h * 0.22, gameOverReturnRect.h * 0.55);
}

void gameOverWindowGestion(GameState *gameState, Mix_Chunk **sounds, SDL_Point mouse) {
    if (SDL_PointInRect(&mouse, &gameOverReturnRect)) {
        Mix_PlayChannel(1, sounds[7], 0);
        setWindowType(NO_WINDOW);
        *gameState = LANDING_PAGE;  // Retour au menu principal
    }
}

void saveGame(Ship *ships, int shipCount, Planet *planets, int planetCount) {
    mkdir("backups", 0755);  // Cree le dossier de sauvegarde s'il n'existe pas

    FILE *backup = fopen(NAME_BACKUP, "wb");  // Ouvre le fichier en mode binaire
    if (!backup) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }
    
    setvbuf(backup, NULL, _IONBF, 0);
    
    // Sauvegarder le nombre de fusees/planetes au début du fichier
    fwrite(&shipCount, sizeof(int), 1, backup);
    fwrite(&planetCount, sizeof(int), 1, backup);

    // Sauvegarder chaque fusee/planete/tuile dans le fichier
    fwrite(ships, sizeof(Ship), shipCount, backup);
    fwrite(planets, sizeof(Planet), planetCount, backup);
    fwrite(*getTilesMatrix(), sizeof(uint8_t), *getByteCount(), backup);

    // Sauvegarde de la meta-progression (credits, score, ameliorations)
    fwrite(getMeta(), sizeof(Meta), 1, backup);

    // Fermeture du fichier de sauvegarde
    fclose(backup);
}
