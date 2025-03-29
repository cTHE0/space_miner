#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

// États possibles du jeu
typedef enum {
    MENU,
    JEU,
    QUITTER
} EtatJeu;

// Affiche le menu principal
void afficherMenu(SDL_Renderer *renderer);

// Gère les événements du menu
void handleMenuEvents(int *running, EtatJeu *etat);

#endif
