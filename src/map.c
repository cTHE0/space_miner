#include "map.h"

#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "renderer.h"


void renderMap(SDL_Texture *textureMap) {
    SDL_Rect destRect;

    if (getCameraScale() > 0.001 ) {
        destRect.w = MAP_SIZE * getCameraScale() / 100 ;  // Largeur adaptee au zoom
        destRect.h = MAP_SIZE * getCameraScale() / 100;  // Hauteur adaptee au zoom

        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                // Appliquer le decalage et le zoom de la camera
                destRect.x = ((i * 3500 - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f);
                destRect.y = ((j * 3500 - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f);

                SDL_RenderCopy(renderer, textureMap, NULL, &destRect);
            }
        }
    }
    
}
