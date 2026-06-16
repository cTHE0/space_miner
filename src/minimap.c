#include "minimap.h"

#include <SDL2/SDL.h>
#include "config.h"
#include "renderer.h"
#include "camera.h"
#include "window.h"
#include "tools.h"
#include "assets_gestion.h"


#define MINIMAP_SIZE (SCREEN_HEIGHT * 0.22)
#define MINIMAP_MARGIN (SCREEN_HEIGHT * 0.02)


static SDL_Rect minimapRect(void) {
    return (SDL_Rect){SCREEN_WIDTH - (int)MINIMAP_SIZE - (int)MINIMAP_MARGIN,
                      SCREEN_HEIGHT - (int)MINIMAP_SIZE - (int)MINIMAP_MARGIN,
                      (int)MINIMAP_SIZE, (int)MINIMAP_SIZE};
}

static int minimapVisible(void) {
    WindowType t = getWindowType();
    return t == NO_WINDOW || t == BASIC_SHIP_WINDOW || t == SIDE_BAR_WINDOW;
}

static SDL_Point mapToMinimap(SDL_Rect box, float mapX, float mapY) {
    float mapSize = MAP_SIZE;  // Variable locale (la macro MAP_SIZE n'est pas parenthesee)
    return (SDL_Point){box.x + (int)(mapX / mapSize * box.w),
                       box.y + (int)(mapY / mapSize * box.h)};
}

void displayMinimap(Ship *ships, int shipCount, Planet *planets, int planetCount) {
    if (!minimapVisible()) return;

    SDL_Rect box = minimapRect();

    // Fond
    SDL_SetRenderDrawColor(renderer, 8, 14, 22, 220);
    SDL_RenderFillRect(renderer, &box);

    // Planetes et soleils
    for (int i = 0; i < planetCount; i++) {
        SDL_Point p = mapToMinimap(box, planets[i].x, planets[i].y);
        if (!SDL_PointInRect(&p, &box)) continue;
        int r = (planets[i].planetType == SUN) ? 2 : 1;
        if (planets[i].planetType == SUN) SDL_SetRenderDrawColor(renderer, 255, 210, 60, 255);
        else SDL_SetRenderDrawColor(renderer, 120, 160, 220, 255);
        SDL_Rect dot = {p.x - r, p.y - r, 2 * r + 1, 2 * r + 1};
        SDL_RenderFillRect(renderer, &dot);
    }

    // Fusees (vert) et pirates (rouge)
    for (int i = 0; i < shipCount; i++) {
        SDL_Point p = mapToMinimap(box, ships[i].x, ships[i].y);
        if (!SDL_PointInRect(&p, &box)) continue;
        if (ships[i].shiptype == ENEMY) SDL_SetRenderDrawColor(renderer, 235, 60, 60, 255);
        else SDL_SetRenderDrawColor(renderer, 60, 230, 90, 255);
        SDL_Rect dot = {p.x, p.y, 2, 2};
        SDL_RenderFillRect(renderer, &dot);
    }

    // Cadre de la camera
    float mapSize = MAP_SIZE;
    SDL_Rect cam = getCameraRect();
    SDL_Point tl = mapToMinimap(box, cam.x, cam.y);
    SDL_Rect view = {tl.x, tl.y, (int)(cam.w / mapSize * box.w), (int)(cam.h / mapSize * box.h)};
    // Clamp visuel dans la box
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &view);

    // Bordure
    SDL_DrawEdgeOfRect(box, 2, WHITE);
}

int minimapClick(SDL_Point mouse) {
    if (!minimapVisible()) return 0;

    SDL_Rect box = minimapRect();
    if (!SDL_PointInRect(&mouse, &box)) return 0;

    // Conversion clic minimap -> coordonnees map, puis recentrage camera
    float mapSize = MAP_SIZE;
    float mapX = (mouse.x - box.x) / (float)box.w * mapSize;
    float mapY = (mouse.y - box.y) / (float)box.h * mapSize;
    setCameraMode(NORMAL);  // On arrete tout suivi pour aller a l'endroit clique
    setCenterCamera((SDL_Point){(int)mapX, (int)mapY});
    return 1;
}
