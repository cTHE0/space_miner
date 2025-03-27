#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "config.h"
#include "camera.h"

void renderMap(SDL_Renderer *renderer, SDL_Texture *textureMap) {
    SDL_Rect destRect;
    
    // Appliquer le décalage et le zoom de la caméra
    destRect.x = ((0 - camera.rect.x - SCREEN_WIDTH / 2) * camera.scale + SCREEN_WIDTH / 2);
    destRect.y = ((0 - camera.rect.y - SCREEN_HEIGHT / 2) * camera.scale + SCREEN_HEIGHT / 2);
    destRect.w = 3500 * camera.scale;  // Largeur adaptée au zoom
    destRect.h = 3500 * camera.scale; // Hauteur adaptée au zoom

    SDL_RenderCopy(renderer, textureMap, NULL, &destRect);
}
