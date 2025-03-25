#include "renderer.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void initRenderer(SDL_Window **window, SDL_Renderer **renderer) {
    
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !IMG_Init(IMG_INIT_PNG)) {
        printf("Erreur SDL : %s\n", SDL_GetError());
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

void presentScreen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}
