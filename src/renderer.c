#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "planet.h"
#include "ship.h"
#include "map.h"
#include "config.h"
#include "window.h"


SDL_Renderer *renderer = NULL;


void initSDL(SDL_Window **window) {
    // Initialiser SDL (vidéo)
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

    // Initialiser SDL_ttf (pour les polices)
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        IMG_Quit();  // Nettoyer SDL_image
        SDL_Quit();  // Nettoyer SDL
        return;
    }

    // Créer la fenêtre
    *window = SDL_CreateWindow("SPACE MINER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        TTF_Quit();  // Nettoyer SDL_ttf
        IMG_Quit();  // Nettoyer SDL_image
        SDL_Quit();  // Nettoyer SDL
        return;
    }

    // Créer le renderer
    renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);  // Nettoyer la fenêtre
        TTF_Quit();  // Nettoyer SDL_ttf
        IMG_Quit();  // Nettoyer SDL_image
        SDL_Quit();  // Nettoyer SDL
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void quitSDL(SDL_Window *window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void clearScreen(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void displayGame(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount, Planet *planets, int planetCount, int pointCount, int nbPointsPerLigne) {
    clearScreen();
    renderMap(imageTextures[3][2]);
    renderPoints(imageTextures, planets, planetCount, pointCount, nbPointsPerLigne);
    renderPlanets(imageTextures, planets, planetCount);
    renderShips(imageTextures, ships, shipCount);
    displayWindow(imageTextures, textTextures, ships, planets, planetCount);
    SDL_RenderPresent(renderer);
}

