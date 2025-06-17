#include "map.h"

#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "renderer.h"


void renderMap(SDL_Texture ***imageTextures) {
    SDL_Rect destRect;


    if (getCameraScale() > 0.025 ) {
        destRect.w = MAP_SIZE * getCameraScale() / 100 ;  // Largeur adaptee au zoom
        destRect.h = MAP_SIZE * getCameraScale() / 100;  // Hauteur adaptee au zoom

        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                // Appliquer le decalage et le zoom de la camera
                destRect.x = ((i * 7000 - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f);
                destRect.y = ((j * 7000 - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f);

                SDL_RenderCopy(renderer, imageTextures[3][4], NULL, &destRect);
            }
        }
    }
    else {
        destRect.w = MAP_SIZE * getCameraScale() / 100 ;  // Largeur adaptee au zoom
        destRect.h = MAP_SIZE * getCameraScale() / 100;  // Hauteur adaptee au zoom

        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                // Appliquer le decalage et le zoom de la camera
                destRect.x = ((i * 7000 - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f);
                destRect.y = ((j * 7000 - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f);

                SDL_RenderCopy(renderer, imageTextures[3][3], NULL, &destRect);
            }
        }
    }   
}
