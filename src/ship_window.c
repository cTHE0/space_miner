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

int nb_ships_to_be_describe = 0;
Ship *ships_to_be_describe = NULL;

void addShipWindows() {
    nb_ships_to_be_describe += 1;
    Ship *temp = realloc(ships_to_be_describe, nb_ships_to_be_describe * sizeof(Ship));
    if (temp == NULL) {
        fprintf(stderr, "Erreur de réallocation de mémoire\n");
        return;
    }
    ships_to_be_describe = temp;
}

void deleteShipWindows() {
    if (nb_ships_to_be_describe == 0) {
        printf("Erreur, il n'y a plus de fenêtre à supprimer !!\n");
        return;
    }
    nb_ships_to_be_describe -= 1;
    if (nb_ships_to_be_describe == 0) {
        free(ships_to_be_describe);
        ships_to_be_describe = NULL;
    } else {
        Ship *temp = realloc(ships_to_be_describe, nb_ships_to_be_describe * sizeof(Ship));
        if (temp == NULL) {
            fprintf(stderr, "Erreur de réallocation de mémoire\n");
            return;
        }
        ships_to_be_describe = temp;
    }
}

void renderShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Ship ship) {
    SDL_Rect window_rect = {400, 250, 800, 600};
    int cross_size = window_rect.w * 0.03;
    SDL_Rect cross_rect = {window_rect.x + window_rect.w - cross_size - 15, window_rect.y + 15, cross_size, cross_size};
    SDL_RenderCopy(renderer, imageTextures[21], NULL, &window_rect);
    SDL_RenderCopy(renderer, imageTextures[22], NULL, &cross_rect);

}

void renderShipWindows(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures) {
    for (int i = 0; i < nb_ships_to_be_describe; i++) {
        renderShipWindow(renderer, imageTextures, textTextures, ships_to_be_describe[i]);
    }
}

void openCloseShipWindowsGestion(int x, int y, int ship_count, Ship *ships) {
    SDL_Point mouse = {x, y};
    for (int i = 0; i<ship_count; i++) {
        if (SDL_PointInFRect(&mouse, &ships[i].destRect)) {
            addShipWindows();
        }
    }
}

void freeWindows() {
    free(ships_to_be_describe);
}