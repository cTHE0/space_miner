#ifndef LANDING_PAGE_H
#define LANDING_PAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "ship.h"
#include "planet.h"


typedef enum {  // etats possibles du jeu
    LANDING_PAGE,
    GAME,
    QUIT
} GameState;

typedef struct {
    int frameIndex1;  // Numero de l'image prise dans le spritesheet de la planete du menu
    int frameIndex2;  // Permet de gerer la rotation est le deplacement des meteorites
    Uint32 lastFrameTime1;   // Date au dernier changement de frame du spritesheet de la planete du menu
    Uint32 lastFrameTime2;   // Date au dernier changement de frame du spritesheet des asteroide du menu
} lpFrameControler;


void initLandingPageRects(void);

void displayMenu(SDL_Texture ***imageTextures, SDL_Texture **textTextures);

void updateFrameIndex(void);

void handleMenuEvents(Mix_Chunk **sounds, GameState *gameState, short *gameBegun, Ship **ships, int shipCount, Planet **planets, int planetCount);

#endif
