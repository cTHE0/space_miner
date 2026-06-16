#include "objectives.h"

#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "text.h"
#include "config.h"
#include "tools.h"
#include "meta.h"
#include "build.h"
#include "notify.h"
#include "assets_gestion.h"
#include "window.h"


typedef struct {
    const char *text;
    int reward;
} Objective;


static const Objective objectives[] = {
    {"Build your first mine",        200},
    {"Build a transporter ship",     250},
    {"Mine 3000 minerals",           300},
    {"Build a defence tower",        300},
    {"Destroy 3 pirates",            400},
    {"Reach 3000 score",             500},
    {"Buy a global upgrade",         500},
    {"Reach 25000 score (VICTORY)", 1000},
};
static const int OBJECTIVE_COUNT = (int)(sizeof(objectives) / sizeof(objectives[0]));

static TTF_Font *objFont = NULL;
static SDL_Texture *objTexture = NULL;
static int builtForIndex = -1;


void initObjectives(TTF_Font *font) {
    objFont = font;
    if (objTexture) { SDL_DestroyTexture(objTexture); objTexture = NULL; }
    builtForIndex = -1;
}

static int objectiveDone(int index, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    Meta *m = getMeta();
    switch (index) {
        case 0:  // une mine construite
            for (int i = 0; i < planetCount; i++)
                for (int b = 3; b < BUILD_TYPE_COUNT; b += 2)
                    if (planets[i].builds[b].type == ORE_MINE && planets[i].builds[b].level > 0) return 1;
            return 0;
        case 1:  // un transporteur fabrique
            return m->shipsBuilt >= 1;
        case 2:
            return m->mineralsMined >= 3000;
        case 3:  // une tour de defense
            for (int i = 0; i < planetCount; i++)
                if (planets[i].builds[1].type == DEFENCE_TOWER && planets[i].builds[1].level > 0) return 1;
            return 0;
        case 4:
            return m->piratesKilled >= 3;
        case 5:
            return m->score >= 3000;
        case 6: {  // au moins une amelioration globale achetee
            for (int u = 0; u < UPG_COUNT; u++) if (upgradeLevel((UpgradeType)u) > 0) return 1;
            return 0;
        }
        case 7:
            return m->score >= VICTORY_SCORE;
        default:
            return 0;
    }
    (void)ships; (void)shipCount;
}

static void rebuildTexture(void) {
    Meta *m = getMeta();
    int idx = m->objectiveIndex;

    if (objTexture) { SDL_DestroyTexture(objTexture); objTexture = NULL; }

    char buf[128];
    if (idx >= OBJECTIVE_COUNT) {
        snprintf(buf, sizeof(buf), "All objectives complete!");
    } else {
        snprintf(buf, sizeof(buf), "Objective: %s  (+%d cr)", objectives[idx].text, objectives[idx].reward);
    }
    if (objFont) {
        objTexture = createTextTexture(objFont, GOLD, buf);
    }
    builtForIndex = idx;
}

void updateObjectives(Ship *ships, int shipCount, Planet *planets, int planetCount) {
    Meta *m = getMeta();
    if (m->objectiveIndex >= OBJECTIVE_COUNT) return;

    if (objectiveDone(m->objectiveIndex, ships, shipCount, planets, planetCount)) {
        addCredits(objectives[m->objectiveIndex].reward);
        addScore(objectives[m->objectiveIndex].reward / 2);
        char msg[96];
        snprintf(msg, sizeof(msg), "Objective complete!  +%d credits", objectives[m->objectiveIndex].reward);
        pushNotification(msg, GOLD);
        m->objectiveIndex++;
    }
}

void displayObjectiveHUD(void) {
    WindowType t = getWindowType();
    if (t != NO_WINDOW && t != BASIC_SHIP_WINDOW && t != SIDE_BAR_WINDOW) return;

    if (builtForIndex != getMeta()->objectiveIndex || !objTexture) {
        rebuildTexture();
    }
    if (!objTexture) return;

    int tw, th;
    SDL_QueryTexture(objTexture, NULL, NULL, &tw, &th);
    int h = SCREEN_HEIGHT * 0.030;
    int w = (int)(tw * (h / (float)th));
    int x = SCREEN_WIDTH * 0.012;
    int y = SCREEN_HEIGHT * 0.065;

    SDL_Rect bg = {x - 8, y - 5, w + 16, h + 10};
    SDL_SetRenderDrawColor(renderer, 12, 18, 28, 200);
    SDL_RenderFillRect(renderer, &bg);
    SDL_Rect accent = {bg.x, bg.y, 5, bg.h};
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
    SDL_RenderFillRect(renderer, &accent);

    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(renderer, objTexture, NULL, &dst);
}

void destroyObjectives(void) {
    if (objTexture) { SDL_DestroyTexture(objTexture); objTexture = NULL; }
}
