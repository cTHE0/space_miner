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
#include "renderer.h"
#include "window.h"


int main(void) {
    // Initialisation 
    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    initCamera();

    int shipCount = INIT_SHIP_COUNT;
    Ship* ships = NULL;

    int planetCount = INIT_PLANET_COUNT;
    Planet* planets = NULL;

    SDL_Texture ***imageTextures = loadTextures(renderer);
    SDL_Texture **textTextures = loadTextTextures(renderer);

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
                displayMenu(renderer, imageTextures, textTextures);

                if (state == GAME) {
                    generatePlanets(&planets, planetCount);
                    initShips(&ships, shipCount, planets, planetCount);
                    camera.rect.x = planets[0].x;
                    camera.rect.y = planets[0].y;
                }
                break;
            
            case GAME:

                handleEvents(&state, ships, shipCount, planets, planetCount);

                updateShips(ships, shipCount);

                displayGame(renderer, imageTextures, textTextures, ships, planets, shipCount, planetCount);
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
    destroyImageTextures(imageTextures);
    destroyTextTextures(textTextures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    destroyShips(ships, shipCount);
    free(planets);
    IMG_Quit();
    SDL_Quit();
    return 0;
}