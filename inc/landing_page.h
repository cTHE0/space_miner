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
    int frameIndex2;  // Permet de gerer la rotation est le deplacement des meteorites
    Uint32 lastFrameTime1;   // Date au dernier changement de frame du spritesheet de la planete du menu
    Uint32 lastFrameTime2;   // Date au dernier changement de frame du spritesheet des asteroide du menu
} lpFrameControler;

// Affiche le menu principal i.e: landing page
void displayMenu(SDL_Texture ***imageTextures, SDL_Texture **textTextures);

void updateFrameIndex(void);

// Gère les événements du menu
void handleMenuEvents(GameState *state);

#endif
