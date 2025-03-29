#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

// États possibles du jeu
typedef enum {
    LANDING_PAGE,
    GAME,
    QUIT
} GameState;

// Affiche le menu principal
void afficherMenu(SDL_Renderer *renderer);

// Gère les événements du menu
void handleMenuEvents(GameState *state);

#endif
