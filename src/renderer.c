#include "renderer.h"

void initRenderer(SDL_Window **window, SDL_Renderer **renderer) {
    *window = SDL_CreateWindow("SPACE MINER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 900, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

void clearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void presentScreen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}
