#include "camera.h"
#include "planet.h"
#include "event.h"
#include "renderer.h"
#include "ship.h"

#define PLANET_COUNT 10
#define SHIP_COUNT 5

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    initRenderer(&window, &renderer);
    initCamera();

    Planet planets[PLANET_COUNT];
    generatePlanets(planets, PLANET_COUNT);

    Ship ships[SHIP_COUNT];
    initShips(ships, SHIP_COUNT, planets, PLANET_COUNT);

    int running = 1;
    while (running) {
        handleEvents(&running);
        updateShips(ships, SHIP_COUNT, planets, PLANET_COUNT);

        clearScreen(renderer);
        renderPlanets(renderer, planets, PLANET_COUNT);
        renderShips(renderer, ships, SHIP_COUNT);
        presentScreen(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
