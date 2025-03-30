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
    int frameIndex1;  // Numero de l'image prise dans le spritesheet de la planete du menu
    int frameIndex2;  // Numero de l'image prise dans le spritesheet de l'asteroide 1
    int frameIndex3;  // Numero de l'image prise dans le spritesheet de l'asteroide 2
    Uint32 lastFrameTime1;   // Date au dernier changement de frame du spritesheet de la planete du menu
    Uint32 lastFrameTime2;   // Date au dernier changement de frame du spritesheet des asteroide du menu
} LPFrameControler;

// Affiche le menu principal i.e: landing page
void afficherMenu(SDL_Renderer *renderer, SDL_Texture *map, SDL_Texture *planet, SDL_Texture *asteroid1, SDL_Texture *asteroid2, SDL_Texture *bg_button, SDL_Texture **cstTextTextures);

void updateFrameIndex();

// Gère les événements du menu
void handleMenuEvents(GameState *state);

#endif
