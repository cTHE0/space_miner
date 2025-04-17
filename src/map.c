#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "config.h"
#include "assets_gestion.h"

void renderMap(SDL_Renderer *renderer, SDL_Texture *textureMap) {
    SDL_Rect destRect;

    if (camera.scale > 0.35 ) {
        destRect.w = MAP_SIZE * camera.scale / 100 ;  // Largeur adaptée au zoom
        destRect.h = MAP_SIZE * camera.scale / 100;  // Hauteur adaptée au zoom

        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                // Appliquer le décalage et le zoom de la caméra
                destRect.x = ((i * 3500 - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f);
                destRect.y = ((j * 3500 - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f);

                SDL_RenderCopy(renderer, textureMap, NULL, &destRect);
            }
        }
    }

    
    
    
}
