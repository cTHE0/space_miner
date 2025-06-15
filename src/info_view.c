#include <SDL2/SDL.h>

#include "info_view.h"
#include "assets_gestion.h"
#include "place.h"
#include "renderer.h"
#include "config.h"

static SDL_Rect up_bar_rect = (SDL_Rect){SCREEN_WIDTH * 0.0400, SCREEN_HEIGHT * 0.0300, SCREEN_WIDTH * 0.2420, SCREEN_WIDTH * 0.0290};
static SDL_Rect frist_icon_up_bar_rect = (SDL_Rect){SCREEN_WIDTH * 0.0040, SCREEN_HEIGHT * 0.0390, SCREEN_WIDTH * 0.0150, SCREEN_WIDTH * 0.0150};

void displayInfoView(SDL_Texture ***imageTextures) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 170);  // gris, 50% transparent
    SDL_RenderFillRect(renderer, &up_bar_rect);

    SDL_Rect local_rect = frist_icon_up_bar_rect;
    for (int i=0; i<ORE_TYPE_COUNT; i++) {
        local_rect.x += 3*frist_icon_up_bar_rect.w;
        SDL_RenderCopy(renderer, imageTextures[4][i], NULL, &local_rect);
    }
}