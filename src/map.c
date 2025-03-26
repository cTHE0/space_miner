#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"

void renderMap(SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_RenderCopy(renderer, texture, &camera.rect, NULL);
    return;
}