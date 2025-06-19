#include "map.h"

#include <SDL2/SDL.h>
#include "camera.h"
#include "config.h"
#include "renderer.h"
#include "tools.h"


void renderMap(SDL_Texture ***imageTextures) {
    SDL_Rect destRect = {0, 0, AREA_SIZE * getCameraScale(), AREA_SIZE * getCameraScale()};

    if (getCameraScale() > LIMIT_UNZOOM) {
        // Affichage du fond etoile
        for (int i = 0; i < NUMBER_OF_AREA_PER_WIDTH; i++){
            for (int j = 0; j < NUMBER_OF_AREA_PER_WIDTH; j++){
                // Appliquer le decalage et le zoom de la camera
                destRect.x = ((i * AREA_SIZE - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f);
                destRect.y = ((j * AREA_SIZE - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f);

                SDL_RenderCopy(renderer, imageTextures[3][7], NULL, &destRect);
            }
        }


        // Affichage du quadrillage
        int sizeSide = 500;
        SDL_Point centerHexagon;
        for (int i = (getCameraRect().x + (1 - 1 / getCameraScale()) * SCREEN_WIDTH / 2.f ) / sizeSide / 1.5; 
             i < (getCameraRect().x + SCREEN_WIDTH * 1.5f / getCameraScale()) / sizeSide / 1.5; 
             i++) {
            for (int j = (getCameraRect().y + (1 - 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / sizeSide / sqrt(3); 
                 j < (getCameraRect().y + (1 + 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / sizeSide / sqrt(3) + 0.5; 
                 j++) {
                if (i % 2 == 0) {
                    centerHexagon.x = (sizeSide * 1.5 * i - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
                    centerHexagon.y = (sizeSide * sqrt(3) * j - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
                } else {
                    centerHexagon.x = (sizeSide * 1.5 * i - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
                    centerHexagon.y = (sizeSide * sqrt(3) * (j - 0.5) - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
                }

                drawHexagon(renderer, centerHexagon, sizeSide * getCameraScale());
            }
        }
    } else {
        // Affichage du fond
        SDL_SetRenderDrawColor(renderer, 18, 52, 73, 255);
        SDL_RenderFillRect(renderer, NULL);

        // Affichage des bordures
        for (int i = 0; i < NUMBER_OF_AREA_PER_WIDTH; i++) {
            for (int j = 0; j < NUMBER_OF_AREA_PER_WIDTH; j++ ) {
                // Appliquer le decalage et le zoom de la camera
                destRect.x = (i * AREA_SIZE - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
                destRect.y = (j * AREA_SIZE - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;

                SDL_DrawEdgeOfRect2(renderer, destRect, 3);
            }
        }
    }
}
