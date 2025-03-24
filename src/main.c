#include "camera.h"
#include "planet.h"
#include "event.h"
#include "renderer.h"
#include "ship.h"
//c'est Adam
#define INIT_PLANET_COUNT 50
#define INIT_SHIP_COUNT 10

int main() {
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

    int running = 1;
    while (running) {
        handleEvents(&running);
        updateShips(ships, ship_count, planets, planet_count);

        clearScreen(renderer);
        renderPlanets(renderer, planets, planet_count);
        renderShips(renderer, ships, ship_count);
        presentScreen(renderer);
    }

    // Libération de la mémoire allouée
    free(planets);  // Libération des planètes
    free(ships);    // Libération des vaisseaux

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
