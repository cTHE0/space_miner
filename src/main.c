#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include "camera.h"
#include "planet.h"
#include "event.h"
#include "renderer.h"
#include "ship.h"
#include "assets_gestion.h"
#include "map.h"
#include "config.h"
#include "landing_page.h"
#include "text.h"
#include "window.h"


uint32_t currentSeed = 187;  // Peut prendre des valeurs entre 1 et 2**32-1

int main(void) {
    // Initialisation 
    srand(time(NULL));

    SDL_Window *window;
    initSDL(&window);

    initCamera();

    int shipCount = INIT_SHIP_COUNT;
    int planetCount = INIT_PLANET_COUNT;

    loadFonts();

    SDL_Texture ***imageTextures = loadTextures();
    textTextures = loadTextTextures();

    // Variables pour mesurer les FPS
    Uint32 toShowFPS = SDL_GetTicks();
    Uint32 toLimitFPS;
    int frameCount = 0;

    // Lancement de space_miner
    GameState state = LANDING_PAGE;

    while (state != QUIT) {
        frameCount++;
        toLimitFPS = SDL_GetTicks();

        switch (state) {
            case LANDING_PAGE:
                handleMenuEvents(&state);
                updateFrameIndex();
                displayMenu(imageTextures, textTextures);

                if (state == GAME) {
                    generatePlanets(planetCount);
                    initShips(shipCount, planetCount);
                    camera.rect.x = planets[1].x;
                    camera.rect.y = planets[1].y;
                }
                break;
            
            case GAME:
                handleEvents(&state, shipCount, planetCount);
                updateShips(shipCount);
                displayGame(imageTextures, textTextures, shipCount, planetCount);
                break;
            
            default:
                state = QUIT;
                break;
        }

        // Affichage des FPS
        if (SDL_GetTicks() - toShowFPS >= 2000) {
            printf("FPS: %d\n", (frameCount * 1000) / (SDL_GetTicks() - toShowFPS));
            frameCount = 0;
            toShowFPS = SDL_GetTicks();
        }
            
        // Limite les FPS
        if ((SDL_GetTicks() - toLimitFPS) < 16) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - toLimitFPS));
        }
    }
    
    // Fermeture du programme
    destroyShips(shipCount);
    free(planets);
    destroyImageTextures(imageTextures);
    destroyTextTextures(textTextures);
    quitSDL(window);
    return 0;
}