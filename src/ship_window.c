#include "ship_window.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "ship.h"

int nb_ship_windows = 0;
ShipWindow *ship_windows = NULL;

void addShipWindows() {
    nb_ship_windows += 1;
    ship_windows = realloc(ship_windows, nb_ship_windows * sizeof(ShipWindow));
    if (ship_windows == NULL) {
        fprintf(stderr, "Erreur de réallocation de mémoire\n");
        return;
    }
}

void deleteShipWindows(int x, int y, int i) { // i : numéro de la fenêtre à suppr
    if (nb_ship_windows == 0) {
        printf("Erreur, il n'y a plus de fenêtre à supprimer !!\n");
        return;
    }
    nb_ship_windows -= 1;
    if (nb_ship_windows == 0) {
        free(ship_windows);
        ship_windows= NULL;
    } else {
        ShipWindow *temp = realloc(ship_windows, nb_ship_windows * sizeof(ShipWindow));
        if (temp == NULL) {
            fprintf(stderr, "Erreur de réallocation de mémoire\n");
            return;
        }
        ship_windows = temp;
    }
}

void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, ShipWindow ship_window) {
    SDL_Rect window_rect = {400, 250, 800, 600};
    int cross_size = window_rect.w * 0.03;
    SDL_Rect cross_rect = {window_rect.x + window_rect.w - cross_size - 15, window_rect.y + 15, cross_size, cross_size};
    SDL_RenderCopy(renderer, imageTextures[21], NULL, &window_rect);
    SDL_RenderCopy(renderer, imageTextures[22], NULL, &cross_rect);

}

void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures) {
    for (int i = 0; i < nb_ship_windows; i++) {
        renderShipWindow(renderer, imageTextures, textTextures, ship_windows[i]);
    }
}

void openCloseShipWindowsGestion(int x, int y, int ship_count, Ship *ships) {
    SDL_Point mouse = {x, y};
    for (int i = 0; i<ship_count; i++) {
        if (SDL_PointInRect(&mouse, &ships[i].destRect)) {
            addShipWindows();
            return;
        }
    }
}

void freeWindows() {
    free(ship_windows);
}