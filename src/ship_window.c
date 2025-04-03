#include "ship_window.h"
#include "ship.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

int nb_ship_windows = 0;
ShipWindow *ship_windows = NULL; // ship_windows[0] en background, puis ship_windows[0], ... jusqu'à ship_windows[nb_ship_windows - 1] au 1er plan
int ShipWindow_size = sizeof(ShipWindow);

void addShipWindow(Ship ship) {
    nb_ship_windows += 1;
    ship_windows = realloc(ship_windows, nb_ship_windows * sizeof(ShipWindow));

    SDL_Rect window_rect = {400, 250, 800, 600};
    int cross_size = (int) window_rect.w * 0.03;
    SDL_Rect cross_rect = {window_rect.x + window_rect.w - cross_size - 15, window_rect.y + 15, cross_size, cross_size};

    if (ship_windows == NULL) {
        fprintf(stderr, "Erreur de réallocation de mémoire\n");
        return;
    }
    ShipWindow nv_ship_window = {ship, window_rect, cross_rect};
    ship_windows[nb_ship_windows-1] = nv_ship_window;
}

void deleteShipWindows(int i) { // i : numéro de la fenêtre à suppr
    if (nb_ship_windows == 0) {
        printf("Erreur, il n'y a plus de fenêtre à supprimer !!\n");
        return;
    }
    if (nb_ship_windows == 1) {
        nb_ship_windows = 0;
        free(ship_windows);
        ship_windows= NULL;
    } else {
        ship_windows = supprElemList(ship_windows, &nb_ship_windows, ShipWindow_size, i);
    }
}

void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, ShipWindow ship_window) {
    SDL_RenderCopy(renderer, imageTextures[21], NULL, &ship_window.destRect);
    SDL_RenderCopy(renderer, imageTextures[22], NULL, &ship_window.crossRect);

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
            addShipWindow(ships[i]);
            return;
        }
    }
    for (int i = 0; i<nb_ship_windows; i++) {
        if (SDL_PointInRect(&mouse, &ship_windows[i].crossRect)) {
            deleteShipWindows(i);
            return;
        }
    }
}

void freeWindows() {
    free(ship_windows);
}

int whichWindow(int mouseX, int mouseY) { // Retourne indice ship fenêtre sur laquelle l'user a cliqué ou retourne -1 s'il n'a cliqué sur aucune fenêtre mais sur map
    SDL_Point mouse = {mouseX, mouseY};
    for (int i = 0; i<nb_ship_windows; i++) {
        if (SDL_PointInRect(&mouse, &ship_windows[nb_ship_windows-i-1].destRect)) { //On parcourt à l'envers car la fenetre la plus en bas a pour indice 0, ...
            return i;
        }
    }
    return -1;
}

void moveWindow(int dx, int dy, int i) { // i numéro de la fenêtre à bouger
    ship_windows = moveToEnd(ship_windows, nb_ship_windows, ShipWindow_size, i); //On place d'abord la fenêtre sur laquelle on vient de cliquer au 1er plan

    // Puis on décale la fenêtre et la zone croix pr la fermer
    ship_windows[nb_ship_windows - 1].destRect.x += dx;
    ship_windows[nb_ship_windows - 1].destRect.y += dy;
    ship_windows[nb_ship_windows - 1].crossRect.x += dx;
    ship_windows[nb_ship_windows - 1].crossRect.y += dy;
}