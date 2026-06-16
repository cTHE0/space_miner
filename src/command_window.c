#include "command_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "renderer.h"
#include "config.h"
#include "tools.h"
#include "text.h"
#include "ore.h"
#include "meta.h"
#include "window.h"
#include "notify.h"
#include "assets_gestion.h"
#include "events.h"


#define SELL_BATCH 200   // Quantite de minerai vendue par clic


// Prix de vente par unite (fuel, fer, or, joanium, voidor)
static const int orePrice[ORE_TYPE_COUNT] = {1, 3, 9, 18, 35};

static SDL_Rect panelRect, crossRect;
static SDL_Rect sellRowRect[ORE_TYPE_COUNT];   // zone cliquable "Sell" de chaque minerai
static SDL_Rect upgRowRect[UPG_COUNT];         // zone cliquable "Buy" de chaque amelioration
static SDL_Rect hireDefenderRect;              // bouton de recrutement d'un defenseur
static int upgFirstY, upgGap;                  // disposition de la colonne d'ameliorations

// Indices des textes (text.c) pour les noms d'ameliorations
static const int upgradeNameTex[UPG_COUNT] = {102, 103, 104, 105, 118, 119};


void initCommandWindowRects(void) {
    panelRect = (SDL_Rect){SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.10, SCREEN_WIDTH * 0.70, SCREEN_HEIGHT * 0.80};

    crossRect = (SDL_Rect){panelRect.x + panelRect.w * 0.955, panelRect.y + panelRect.h * 0.02,
                           panelRect.w * 0.03, panelRect.w * 0.03};

    int firstRowY = panelRect.y + panelRect.h * 0.30;
    int rowGap = panelRect.h * 0.13;

    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        sellRowRect[i] = (SDL_Rect){panelRect.x + panelRect.w * 0.30, firstRowY + i * rowGap,
                                    panelRect.w * 0.13, rowGap * 0.62};
    }
    // La colonne d'ameliorations (6 lignes) a son propre espacement, plus serre
    upgFirstY = panelRect.y + panelRect.h * 0.27;
    upgGap = panelRect.h * 0.093;
    for (int i = 0; i < UPG_COUNT; i++) {
        upgRowRect[i] = (SDL_Rect){panelRect.x + panelRect.w * 0.86, upgFirstY + i * upgGap,
                                   panelRect.w * 0.11, upgGap * 0.62};
    }

    hireDefenderRect = (SDL_Rect){panelRect.x + panelRect.w * 0.06, panelRect.y + panelRect.h * 0.90,
                                  panelRect.w * 0.30, panelRect.h * 0.075};
}

static void drawText(SDL_Texture *tex, int x, int y, int h, int centered) {
    if (!tex) return;
    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    int w = (int)(tw * (h / (float)th));
    SDL_Rect r = {centered ? x - w / 2 : x, y, w, h};
    SDL_RenderCopy(renderer, tex, NULL, &r);
}

static void drawButton(SDL_Rect rect, SDL_Texture *label, int enabled) {
    if (enabled) {
        SDL_SetRenderDrawColor(renderer, 59, 198, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    }
    SDL_RenderFillRect(renderer, &rect);
    SDL_DrawEdgeOfRect(rect, 3, BLACK);
    drawText(label, rect.x + rect.w / 2, rect.y + rect.h * 0.18, rect.h * 0.64, 1);
}

void displayCommandWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    Meta *m = getMeta();

    // Fond
    SDL_SetRenderDrawColor(renderer, 18, 52, 73, 255);
    SDL_RenderFillRect(renderer, &panelRect);
    SDL_DrawEdgeOfRect(panelRect, 4, WHITE);

    // Croix de fermeture
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, &crossRect);

    // Titre + credits
    drawText(textTextures[97], panelRect.x + panelRect.w / 2, panelRect.y + panelRect.h * 0.04, panelRect.h * 0.06, 1);
    drawText(textTextures[95], panelRect.x + panelRect.w * 0.40, panelRect.y + panelRect.h * 0.135, panelRect.h * 0.045, 0);
    SDL_Rect credRect = {panelRect.x + panelRect.w * 0.55, panelRect.y + panelRect.h * 0.125, panelRect.w * 0.012, panelRect.h * 0.055};
    renderNumber((int)m->credits, credRect);

    // En-tetes de colonnes
    drawText(textTextures[98], panelRect.x + panelRect.w * 0.04, panelRect.y + panelRect.h * 0.22, panelRect.h * 0.045, 0);
    drawText(textTextures[99], panelRect.x + panelRect.w * 0.55, panelRect.y + panelRect.h * 0.22, panelRect.h * 0.045, 0);

    int firstRowY = panelRect.y + panelRect.h * 0.30;
    int rowGap = panelRect.h * 0.13;
    int iconSz = panelRect.h * 0.07;

    // --- Colonne gauche : vente de minerais ---
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        int y = firstRowY + i * rowGap;

        // Icone du minerai
        SDL_Rect icon = {panelRect.x + panelRect.w * 0.04, y - iconSz * 0.1, iconSz, iconSz};
        SDL_RenderCopy(renderer, imageTextures[4][i], NULL, &icon);

        // Quantite possedee
        SDL_Rect qty = {panelRect.x + panelRect.w * 0.12, y, panelRect.w * 0.011, panelRect.h * 0.05};
        renderNumber(getTotalOreWithInt(i), qty);

        // Prix par unite (petit, sous la quantite)
        SDL_Rect pr = {panelRect.x + panelRect.w * 0.12, y + panelRect.h * 0.055, panelRect.w * 0.007, panelRect.h * 0.032};
        renderNumber(orePrice[i], pr);

        // Bouton Sell
        drawButton(sellRowRect[i], textTextures[100], getTotalOreWithInt(i) > 0);
    }

    // --- Colonne droite : ameliorations globales ---
    for (int i = 0; i < UPG_COUNT; i++) {
        int y = upgFirstY + i * upgGap;

        // Nom de l'amelioration
        drawText(textTextures[upgradeNameTex[i]], panelRect.x + panelRect.w * 0.55, y, panelRect.h * 0.035, 0);

        // Niveau (jauge de points)
        int lvl = upgradeLevel((UpgradeType)i);
        for (int d = 0; d < UPG_MAX_LEVEL; d++) {
            SDL_Rect dot = {panelRect.x + panelRect.w * 0.55 + d * (panelRect.w * 0.016), y + panelRect.h * 0.042,
                            panelRect.w * 0.011, panelRect.h * 0.022};
            if (d < lvl) SDL_SetRenderDrawColor(renderer, 59, 198, 0, 255);
            else SDL_SetRenderDrawColor(renderer, 70, 80, 90, 255);
            SDL_RenderFillRect(renderer, &dot);
            SDL_DrawEdgeOfRect(dot, 1, BLACK);
        }

        // Cout du prochain niveau
        if (lvl < UPG_MAX_LEVEL) {
            SDL_Rect cost = {panelRect.x + panelRect.w * 0.74, y, panelRect.w * 0.010, panelRect.h * 0.045};
            renderNumber(upgradeCost((UpgradeType)i), cost);
            drawButton(upgRowRect[i], textTextures[101], m->credits >= upgradeCost((UpgradeType)i));
        } else {
            drawButton(upgRowRect[i], textTextures[101], 0);  // Niveau max atteint
        }
    }

    // Bouton de recrutement d'un defenseur (chasse les pirates)
    drawButton(hireDefenderRect, textTextures[117], m->credits >= DEFENDER_COST);
    SDL_Rect dcost = {hireDefenderRect.x + hireDefenderRect.w + panelRect.w * 0.01, hireDefenderRect.y + hireDefenderRect.h * 0.2,
                      panelRect.w * 0.012, hireDefenderRect.h * 0.55};
    renderNumber(DEFENDER_COST, dcost);
}

void commandWindowGestion(Mix_Chunk **sounds, Ship **ships, int *shipCount, Planet *planets, int planetCount, SDL_Point mouse) {
    // Fermeture
    if (SDL_PointInRect(&mouse, &crossRect) || !SDL_PointInRect(&mouse, &panelRect)) {
        setWindowType(NO_WINDOW);
        Mix_PlayChannel(1, sounds[10], 0);
        return;
    }

    // Recrutement d'un defenseur (apparait sur une planete colonisee)
    if (SDL_PointInRect(&mouse, &hireDefenderRect)) {
        if (getMeta()->credits >= DEFENDER_COST) {
            int home = -1;
            for (int i = 0; i < planetCount; i++) {
                if (planets[i].planetType != SUN) {
                    for (int b = 0; b < BUILD_TYPE_COUNT; b++) {
                        if (planets[i].builds[b].level > 0) { home = i; break; }
                    }
                }
                if (home != -1) break;
            }
            if (home != -1) {
                spendCredits(DEFENDER_COST);
                addDefender(ships, shipCount, &planets[home]);
                Mix_PlayChannel(1, sounds[5], 0);
            } else {
                Mix_PlayChannel(1, sounds[8], 0);
                pushNotification("No colony to deploy a defender!", RED);
            }
        } else {
            Mix_PlayChannel(1, sounds[8], 0);
            pushNotification("Not enough credits!", RED);
        }
        return;
    }

    // Vente de minerais
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        if (SDL_PointInRect(&mouse, &sellRowRect[i])) {
            int drained = drainTotalOre(planets, planetCount, (Ore)i, SELL_BATCH);
            if (drained > 0) {
                int gain = (int)(drained * orePrice[i] * eventMarketMultiplier());
                addCredits(gain);
                addScore(gain / 4);
                Mix_PlayChannel(1, sounds[2], 0);  // Son "bonus gagne"
                char msg[64];
                snprintf(msg, sizeof(msg), "Sold %d ore for %d credits", drained, gain);
                pushNotification(msg, GREEN2);
            } else {
                Mix_PlayChannel(1, sounds[8], 0);
                pushNotification("Nothing to sell!", RED);
            }
            return;
        }
    }

    // Achat d'ameliorations
    for (int i = 0; i < UPG_COUNT; i++) {
        if (SDL_PointInRect(&mouse, &upgRowRect[i])) {
            if (buyUpgrade((UpgradeType)i)) {
                Mix_PlayChannel(1, sounds[5], 0);
                pushNotification("Upgrade purchased!", GREEN);
            } else {
                Mix_PlayChannel(1, sounds[8], 0);
                pushNotification("Cannot afford upgrade!", RED);
            }
            return;
        }
    }
}
