#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ship.h"
#include "planet.h"
#include "map.h"
#include "renderer.h"
#include "config.h"
#include "window.h"

Window windowOpened = NO_WINDOW;

void initRenderer(SDL_Window **window, SDL_Renderer **renderer) {
    
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !IMG_Init(IMG_INIT_PNG)) {
        printf("Erreur initialisation de SDL : %s\n", SDL_GetError());
        return;
    }

    // Initialiser window et renderer
    *window = SDL_CreateWindow("SPACE MINER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("Erreur de création de fenêtre ou du rendu : %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return;
    }
}

void clearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void displayGame(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int shipCount, int planetCount) {
    clearScreen(renderer);
    renderMap(renderer, imageTextures[16]);
    renderPlanets(renderer, planets, imageTextures[8], planetCount);
    renderShips(renderer, imageTextures[0], ships, shipCount);
    afficheShipWindow(renderer, imageTextures, textTextures);
    affichePlanetWindow(renderer, imageTextures, textTextures);
    SDL_RenderPresent(renderer);
}
