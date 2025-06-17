#include <SDL2/SDL.h>
#include "side_bar_window.h"
#include "place.h"
#include "renderer.h"
#include "window.h"
#include "info_view.h"

SDL_Rect openSideBarRect = (SDL_Rect){SCREEN_WIDTH * 0.8636, SCREEN_HEIGHT * 0.0520, SCREEN_WIDTH * 0.1370, SCREEN_WIDTH * 0.5280};

void clickSideBar(SDL_Point mouse) {
    SDL_Rect sideBarButtonRect = getSideBarButtonRect();
    if (SDL_PointInRect(&mouse, &sideBarButtonRect)) {
        setWindowType(SIDE_BAR_WINDOW);
    }
}

void displaySideBar() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 170);
    SDL_RenderFillRect(renderer, &openSideBarRect);
}