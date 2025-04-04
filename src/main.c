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
#include "ship_window.h"

int main(void) {
    // Initialisation 
    int planet_count = INIT_PLANET_COUNT;
    int ship_count = INIT_SHIP_COUNT;

    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    initRenderer(&window, &renderer);

    initCamera();
    init_SDL_ttf();

    Planet* planets = NULL;
    generatePlanets(&planets, planet_count);

    Ship* ships = NULL;
    initShips(&ships, ship_count, planets, planet_count);

    SDL_Texture **imageTextures = loadTextures(renderer);
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
                handleEvents(&state, ship_count, ships);

                updateShips(ships, ship_count);

                displayGame(renderer, imageTextures, textTextures, ships, planets, ship_count, planet_count);
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
    destroyShips(ships, ship_count);
    free(planets);
    IMG_Quit();
    SDL_Quit();
    return 0;
}