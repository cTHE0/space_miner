#include "notify.h"

#include <string.h>
#include <SDL2/SDL.h>
#include "renderer.h"
#include "text.h"
#include "config.h"
#include "tools.h"
#include "assets_gestion.h"

#define NOTIF_LIFETIME 4000u   // Duree d'affichage (ms)
#define NOTIF_FADE 700u        // Duree du fondu de sortie (ms)


typedef struct {
    char text[96];
    SDL_Color color;
    SDL_Texture *texture;
    Uint32 bornTime;
    int active;
} Notification;


static Notification notifications[MAX_NOTIFICATIONS];
static TTF_Font *notifyFont = NULL;


void initNotify(TTF_Font *font) {
    notifyFont = font;
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        notifications[i].texture = NULL;
        notifications[i].active = 0;
    }
}

static void freeSlot(int i) {
    if (notifications[i].texture) {
        SDL_DestroyTexture(notifications[i].texture);
        notifications[i].texture = NULL;
    }
    notifications[i].active = 0;
}

void pushNotification(const char *text, SDL_Color color) {
    if (!text || !notifyFont) return;

    // Trouve un emplacement libre, sinon recycle le plus ancien
    int slot = -1;
    Uint32 oldest = 0xFFFFFFFFu;
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        if (!notifications[i].active) { slot = i; break; }
        if (notifications[i].bornTime < oldest) { oldest = notifications[i].bornTime; slot = i; }
    }
    if (slot == -1) slot = 0;

    freeSlot(slot);

    strncpy(notifications[slot].text, text, sizeof(notifications[slot].text) - 1);
    notifications[slot].text[sizeof(notifications[slot].text) - 1] = '\0';
    notifications[slot].color = color;
    notifications[slot].bornTime = SDL_GetTicks();
    notifications[slot].texture = createTextTexture(notifyFont, color, notifications[slot].text);
    notifications[slot].active = 1;
}

void updateNotifications(void) {
    Uint32 now = SDL_GetTicks();
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        if (notifications[i].active && now - notifications[i].bornTime > NOTIF_LIFETIME) {
            freeSlot(i);
        }
    }
}

void displayNotifications(void) {
    Uint32 now = SDL_GetTicks();

    // Rassemble les notifications actives, de la plus recente a la plus ancienne
    int order[MAX_NOTIFICATIONS];
    int count = 0;
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        if (notifications[i].active && notifications[i].texture) {
            order[count++] = i;
        }
    }
    // Tri par anciennete (recent en bas)
    for (int a = 0; a < count; a++) {
        for (int b = a + 1; b < count; b++) {
            if (notifications[order[a]].bornTime > notifications[order[b]].bornTime) {
                int t = order[a]; order[a] = order[b]; order[b] = t;
            }
        }
    }

    int rowH = SCREEN_HEIGHT * 0.045;
    int baseY = SCREEN_HEIGHT * 0.86;

    for (int k = 0; k < count; k++) {
        int i = order[k];
        Uint32 age = now - notifications[i].bornTime;
        Uint8 alpha = 255;
        if (age > NOTIF_LIFETIME - NOTIF_FADE) {
            alpha = (Uint8)(255 * (NOTIF_LIFETIME - age) / (float)NOTIF_FADE);
        }

        int tw, th;
        SDL_QueryTexture(notifications[i].texture, NULL, NULL, &tw, &th);
        int h = rowH;
        int w = (int)(tw * (h * 0.62) / (float)th);
        int x = SCREEN_WIDTH / 2 - w / 2;
        int y = baseY - k * (rowH + SCREEN_HEIGHT * 0.012);

        // Fond translucide
        SDL_Rect bg = {x - 18, y - 6, w + 36, h};
        SDL_SetRenderDrawColor(renderer, 12, 18, 28, (Uint8)(alpha * 0.72f));
        SDL_RenderFillRect(renderer, &bg);

        // Liseré coloré à gauche
        SDL_Rect accent = {bg.x, bg.y, 5, bg.h};
        SDL_SetRenderDrawColor(renderer, notifications[i].color.r, notifications[i].color.g, notifications[i].color.b, alpha);
        SDL_RenderFillRect(renderer, &accent);

        // Texte
        SDL_Rect dst = {x, y, w, (int)(h * 0.62)};
        SDL_SetTextureAlphaMod(notifications[i].texture, alpha);
        SDL_RenderCopy(renderer, notifications[i].texture, NULL, &dst);
    }
}

void destroyNotify(void) {
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        freeSlot(i);
    }
}
