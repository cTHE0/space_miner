#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "config.h"
#include "camera.h"

void renderMap(SDL_Renderer *renderer, SDL_Texture *textureMap) {
    SDL_Rect destRect;
    
    // Appliquer le décalage et le zoom de la caméra
    destRect.x = ((0 - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f);
    destRect.y = ((0 - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f);
    destRect.w = MAP_SIZE * camera.scale;  // Largeur adaptée au zoom
    destRect.h = MAP_SIZE * camera.scale;  // Hauteur adaptée au zoom

    SDL_RenderCopy(renderer, textureMap, NULL, &destRect);
}
