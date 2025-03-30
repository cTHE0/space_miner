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

    SDL_Texture **textures = loadTextures(renderer);
    SDL_Texture **cstTextTextures = loadTextTextures(renderer);

    // Lancement de space_miner
    GameState state = LANDING_PAGE;

    while (state != QUIT) {
        switch (state)
        {
        case LANDING_PAGE:
            handleMenuEvents(&state);
            updateFrameIndex();
            displayMenu(renderer, textures[16] , textures[17], textures[18], textures[19], textures[20], cstTextTextures);
            break;
        
        case GAME:
            handleEvents(&state);

            updateShips(ships, ship_count, planets, planet_count);

            clearScreen(renderer);
            renderMap(renderer, textures[16]);
            renderPlanets(renderer, planets, textures[8], planet_count);
            renderShips(renderer, textures[0], ships, ship_count);
            presentScreen(renderer);
            break;
        
        default:
            state = QUIT;
            break;
        }
    }
    
    // Fermeture du programme
    free(planets);
    free(ships);
    SDL_DestroyTextures(textures);
    destroyTexts(cstTextTextures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
