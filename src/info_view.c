#include <SDL2/SDL.h>

#include "info_view.h"
#include "assets_gestion.h"
#include "place.h"
#include "renderer.h"
#include "config.h"
#include "text.h"
#include "ore.h"

static SDL_Rect up_bar_rect = (SDL_Rect){SCREEN_WIDTH * -0.0000, SCREEN_HEIGHT * -0.0450, SCREEN_WIDTH * 1.0000, SCREEN_WIDTH * 0.0500};
static SDL_Rect up_values_rect;
static SDL_Rect right_bar_rect = (SDL_Rect){SCREEN_WIDTH * 0.9820, SCREEN_HEIGHT * 0.4250, SCREEN_WIDTH * 0.0190, SCREEN_WIDTH * 0.0860};
static SDL_Rect arrow_right_bar_rect = (SDL_Rect){SCREEN_WIDTH * 0.9840, SCREEN_HEIGHT * 0.4890, SCREEN_WIDTH * 0.0150, SCREEN_WIDTH * 0.0140};
static SDL_Rect frist_icon_up_bar_rect = (SDL_Rect){SCREEN_WIDTH * -0.0910, SCREEN_HEIGHT * 0.0070, SCREEN_WIDTH * 0.0170, SCREEN_WIDTH * 0.0180};
static SDL_Rect settings_rect = (SDL_Rect){SCREEN_WIDTH * 0.9540, SCREEN_HEIGHT * 0.0040, SCREEN_WIDTH * 0.0210, SCREEN_WIDTH * 0.0200};

void displayInfoView(SDL_Texture ***imageTextures) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 170);  // gris, 50% transparent
    SDL_RenderFillRect(renderer, &up_bar_rect);

    SDL_Rect local_rect = frist_icon_up_bar_rect;
    for (int i=0; i<ORE_TYPE_COUNT; i++) {
        local_rect.x += 6*frist_icon_up_bar_rect.w;
        SDL_RenderCopy(renderer, imageTextures[4][i], NULL, &local_rect);
    }

    SDL_RenderCopy(renderer, imageTextures[2][7], NULL, &settings_rect);
    SDL_RenderFillRect(renderer, &right_bar_rect);
    SDL_RenderCopy(renderer, imageTextures[2][2], NULL, &arrow_right_bar_rect);

    up_values_rect = (SDL_Rect){SCREEN_WIDTH * getEmp().x, SCREEN_HEIGHT * getEmp().y, SCREEN_WIDTH * getEmp().w, SCREEN_WIDTH * getEmp().h};
    local_rect = up_values_rect;
    for (int i=0; i<ORE_TYPE_COUNT; i++) {
        renderNumber(renderer, getTotalOreWithInt(i), local_rect);
        local_rect.x += 6*up_values_rect.w;
    }
}