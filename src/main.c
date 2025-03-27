#include "camera.h"
#include "planet.h"
#include "event.h"
#include "renderer.h"
#include "ship.h"
#include "assets_gestion.h"
#include "map.h"
#include "time.h"

#define INIT_PLANET_COUNT 150
#define INIT_SHIP_COUNT 1200

int main(void) {
    // Initialisation 
    int planet_count = INIT_PLANET_COUNT;
    int ship_count = INIT_SHIP_COUNT;

    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    initRenderer(&window, &renderer);

    initCamera();

    Planet* planets = NULL;
    generatePlanets(&planets, planet_count);

    Ship* ships = NULL;
    initShips(&ships, ship_count, planets, planet_count);

    SDL_Texture **textures = loadTextures(renderer);


    // Lancement de space_miner
    int running = 1;
    while (running) {
        handleEvents(&running);
        
        updateShips(ships, ship_count, planets, planet_count);

        clearScreen(renderer);
        renderMap(renderer, textures[13]);
        renderPlanets(renderer, planets, textures[8], planet_count);
        renderShips(renderer, textures[0], ships, ship_count);
        presentScreen(renderer);
    }


    // Fermeture du programme
    free(planets);
    free(ships);
    SDL_DestroyTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
