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

int which_ship_is_viewed = -1;
SDL_Rect window_rect = {(1 - 0.8) * SCREEN_WIDTH / 2., 
                        (1 - 0.8) * SCREEN_HEIGHT / 2., 
                        SCREEN_WIDTH * 0.8, 
                        SCREEN_HEIGHT * 0.8};
int cross_size;
int intsize;
SDL_Rect cross_rect;

void afficheShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures) {
    cross_size = (int)(window_rect.w * 0.03);
    cross_rect.x = window_rect.x + window_rect.w - cross_size - 15;
    cross_rect.y = window_rect.y + 15;
    cross_rect.w = cross_size;
    cross_rect.h = cross_size;

    if (which_ship_is_viewed != -1) {
        SDL_RenderCopy(renderer, imageTextures[21], NULL, &window_rect);
        SDL_RenderCopy(renderer, imageTextures[22], NULL, &cross_rect);
    }
}


void openWindowGestion(int x, int y, Ship *ships, int nb_ships) {
    SDL_Point mouse = {x, y};
    if (which_ship_is_viewed == -1) {
        for (int i = 0; i<nb_ships; i++) {
            if (SDL_PointInRect(&mouse, &ships[i].destRect)) {
                which_ship_is_viewed = i;
                return;
            }
        }
    } else if (SDL_PointInRect(&mouse, &cross_rect)){
        which_ship_is_viewed = -1;
    }
}
