#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ship.h"
#include "planet.h"
#include "map.h"
#include "renderer.h"
#include "config.h"
#include "window.h"
#include "text.h"

Window windowOpened = NO_WINDOW;

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return;
    }

    // Initialiser SDL_image (pour supporter les images PNG)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
        SDL_Quit();  // Fermer SDL avant de sortir
        return;
    }

    // Créer la fenêtre
    *window = SDL_CreateWindow("SPACE MINER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        IMG_Quit();  // Nettoyer SDL_image
        SDL_Quit();  // Nettoyer SDL
        return;
    }

    // Créer le renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);  // Nettoyer la fenêtre
        IMG_Quit();  // Nettoyer SDL_image
        SDL_Quit();  // Nettoyer SDL
        return;
    }

    init_SDL_ttf();

    return;
}

void clearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void displayGame(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, Planet *planets, int shipCount, int planetCount) {
    clearScreen(renderer);
    renderMap(renderer, imageTextures[3][2]);
    renderPlanets(renderer, planets, imageTextures, planetCount);
    renderShips(renderer, imageTextures, ships, shipCount);
    displayWindow(renderer, imageTextures, textTextures, ships, planets, shipCount, planetCount);
    SDL_RenderPresent(renderer);
}
