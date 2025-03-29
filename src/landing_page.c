#include "landing_page.h"
#include <SDL2/SDL.h>

// Couleur du fond du menu
#define MENU_BACKGROUND_COLOR 0, 0, 50, 255

// Affichage du menu principal
void afficherMenu(SDL_Renderer *renderer) {
    // Efface l'écran avec une couleur de fond spécifique
    SDL_SetRenderDrawColor(renderer, MENU_BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    // Exemple d'ajout de texte ou d'éléments graphiques
    // Pour l'instant, juste une couleur de fond

    // Présente le rendu à l'écran
    SDL_RenderPresent(renderer);
}

// Gère les événements du menu
void handleMenuEvents(GameState *state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *state = QUIT;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:  // Quitter depuis le menu
                    *state = QUIT;
                    break;
                case SDLK_n:       // Nouvelle partie
                    *state = GAME;
                    break;
                case SDLK_c:       // Continuer (à compléter plus tard)
                    *state = GAME;  // Pour l'instant, même effet que "Nouvelle partie"
                    break;
            }
        }
    }
}
