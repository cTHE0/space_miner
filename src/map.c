#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "config.h"
#include "camera.h"

void renderMap(SDL_Renderer *renderer, SDL_Texture *bg_texture) {
    if (!bg_texture) return; // Vérifie que la texture est bien chargée

    SDL_Rect destRect;
    
    
    // Appliquer le décalage et le zoom de la caméra
    destRect.x = ((0 - camera.rect.x) * camera.scale + SCREEN_WIDTH / 2);
    destRect.y = ((0 - camera.rect.y) * camera.scale + SCREEN_HEIGHT / 2);
    destRect.w = SCREEN_WIDTH * camera.scale;  // Largeur adaptée au zoom
    destRect.h = SCREEN_HEIGHT * camera.scale; // Hauteur adaptée au zoom

    SDL_RenderCopy(renderer, bg_texture, NULL, &destRect);

}
