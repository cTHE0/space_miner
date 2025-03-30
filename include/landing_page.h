#ifndef LANDING_PAGE_H
#define LANDING_PAGE_H

#include <SDL2/SDL.h>

// États possibles du jeu
typedef enum {
    LANDING_PAGE,
    GAME,
    QUIT
} GameState;

typedef struct {
    int frameIndex1, frameIndex2, frameIndex3;
    Uint32 lastFrameTime1, lastFrameTime2;
} LPFrameControler;

// Affiche le menu principal i.e: landing page
void afficherMenu(SDL_Renderer *renderer, SDL_Texture *map, SDL_Texture *planet, SDL_Texture *asteroid1, SDL_Texture *asteroid2, SDL_Texture *bg_button, SDL_Texture **cstTextTextures);

void updateFrameIndex();

// Gère les événements du menu
void handleMenuEvents(GameState *state);

#endif
