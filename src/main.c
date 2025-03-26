#include "camera.h"
#include "planet.h"
#include "event.h"
#include "renderer.h"
#include "ship.h"
#include "assets_gestion.h"
#include "map.h"

#define INIT_PLANET_COUNT 50
#define INIT_SHIP_COUNT 10

int main() {
    // Initialisation 
    int planet_count = INIT_PLANET_COUNT;
    int ship_count = INIT_SHIP_COUNT;

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
        renderPlanets(renderer, planets, planet_count);
        renderShips(renderer, textures[0], ships, ship_count);
        presentScreen(renderer);
    }

    // Libération de la mémoire allouée
    free(planets);  // Libération des planètes
    free(ships);    // Libération des vaisseaux

    SDL_DestroyTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
