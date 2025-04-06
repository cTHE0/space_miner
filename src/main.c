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

    int planetCount = INIT_PLANET_COUNT;
    Planet* planets = NULL;
    generatePlanets(&planets, planetCount);

    int shipCount = INIT_SHIP_COUNT;
    Ship* ships = NULL;
    initShips(&ships, shipCount, planets, planetCount);

    SDL_Texture ***imageTextures = loadTextures(renderer);
    SDL_Texture **textTextures = loadTextTextures(renderer);

    // Lancement de space_miner
    GameState state = LANDING_PAGE;

    while (state != QUIT) {
        switch (state) {
            case LANDING_PAGE:
                handleMenuEvents(&state);
                updateFrameIndex();
                displayMenu(renderer, imageTextures, textTextures);
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