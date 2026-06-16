#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"
#include "map.h"
#include "config.h"
#include "window.h"
#include "asteroid.h"
#include "tile.h"
#include "enemy.h"
#include "notify.h"
#include "minimap.h"


SDL_Renderer *renderer = NULL;


void initSDL(SDL_Window **window) {
    // Initialiser SDL (vidéo et audio)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return;
    }

    // Initialiser SDL_image (pour supporter les images PNG)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    // Initialiser SDL_ttf (pour les polices)
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // Initialiser SDL_mixer (pour le son)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur d'initialisation de SDL_mixer : %s\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }
    Mix_Volume(0, MIX_MAX_VOLUME * 0.7);
    Mix_Volume(1, MIX_MAX_VOLUME);

    // Créer la fenêtre
    *window = SDL_CreateWindow("VOID REIGN: THE MINERALS WAR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // Créer le renderer
    renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // Pour pouvoir dessiner avec transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}


void clearScreen(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void displayGame(SDL_Texture ***imageTextures, SDL_Texture **textTextures, Ship *ships, int shipCount, Planet *planets, int planetCount) {
    clearScreen();
    displayMap(imageTextures);
    displayPlanets(imageTextures, planets, planetCount);
    displayShips(imageTextures, ships, shipCount, planets);
    displayLasers(imageTextures);
    displayAsteroid(imageTextures);
    displayTiles(imageTextures);
    displayMinimap(ships, shipCount, planets, planetCount);
    displayWindow(imageTextures, textTextures, ships, planets);
    displaySelectionCircle();
    displayNotifications();
    SDL_RenderPresent(renderer);
}

void quitSDL(SDL_Window *window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}
