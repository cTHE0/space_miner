#include "landing_page.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

LPFrameControler lp_frame_controller = {0, 0, 0, 0, 0};

SDL_Rect bgButton1Rect = {780, 320, 350, 55};  // Position du background des boutons sur le menu
SDL_Rect bgButton2Rect = {780, 420, 350, 55};
SDL_Rect bgButton3Rect = {780, 520, 350, 55};

int bg_button_a_afficher;


void handleMenuEvents(GameState *state) {   // Gère les événements du menu
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        SDL_Point point = {x, y};
        switch (event.type) {

            case SDL_QUIT:       // Quitter depuis le menu
                *state = QUIT;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:      // Quitter depuis le menu
                        *state = QUIT;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                // Vérifier si la souris est dans les zones des boutons
                if (SDL_PointInRect(&point, &bgButton1Rect)) {
                    bg_button_a_afficher = 1;
                } else if (SDL_PointInRect(&point, &bgButton2Rect)) {
                    bg_button_a_afficher = 2;
                } else if (SDL_PointInRect(&point, &bgButton3Rect)) {
                    bg_button_a_afficher = 3;
                } else {
                    bg_button_a_afficher = 0;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (bg_button_a_afficher != 0) {  // Si la souris est dans les zones des boutons
                        if (bg_button_a_afficher == 1 || bg_button_a_afficher == 2) {
                            *state = GAME;
                        }
                    }
                }
                break;


            default:
                break;
        }
    }
}

void updateFrameIndex() {
    if (SDL_GetTicks() > lp_frame_controller.lastFrameTime1 + SHIP_FRAME_DELAY) {
        lp_frame_controller.frameIndex1 = (lp_frame_controller.frameIndex1 + 1) % 160;  // 160 images dans le sprite sheet de la
        lp_frame_controller.lastFrameTime1 = SDL_GetTicks();                            // planetes qui tourne dans le menu
    }
    if (SDL_GetTicks() > lp_frame_controller.lastFrameTime2 + 0.14 * SHIP_FRAME_DELAY) {
        lp_frame_controller.frameIndex2 = (lp_frame_controller.frameIndex2 + 1) % (SCREEN_WIDTH / 4);
        lp_frame_controller.frameIndex3 = (lp_frame_controller.frameIndex3 + 1) % SCREEN_HEIGHT;
        lp_frame_controller.lastFrameTime2 = SDL_GetTicks();
    }
}

void displayMenu(SDL_Renderer *renderer, SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    // Initialisation graphique du menu
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imageTextures[2][0], NULL, NULL);

    // Affichage de la planetes en rotation
    SDL_Rect srcRect = {lp_frame_controller.frameIndex1 * 100, 0, 100, 100}; // Frame actuelle sur le sprite sheet de la planete du menu
    SDL_Rect destRect = {85, 198, 500, 500};  // Position et taille affichee
    SDL_RenderCopy(renderer, imageTextures[1][10], &srcRect, &destRect);

    // Affichages de l'asteroide 1
    SDL_Rect asteroid1Rect = {SCREEN_WIDTH - 4 * lp_frame_controller.frameIndex2, lp_frame_controller.frameIndex3, 50, 50};
    SDL_Point asteroid1Center = {asteroid1Rect.w / 2, asteroid1Rect.h / 2};

    SDL_RenderCopyEx(renderer, imageTextures[5][0], NULL, &asteroid1Rect, lp_frame_controller.frameIndex2, &asteroid1Center, SDL_FLIP_NONE);

    // Affichages de l'asteroide 2
    SDL_Rect asteroid2Rect = {80 + SCREEN_WIDTH - 4 * lp_frame_controller.frameIndex2, 40 + lp_frame_controller.frameIndex3, 40, 40};
    SDL_Point asteroid2Center = {asteroid2Rect.w / 2, asteroid2Rect.h / 2};

    SDL_RenderCopyEx(renderer, imageTextures[5][1], NULL, &asteroid2Rect, lp_frame_controller.frameIndex3, &asteroid2Center, SDL_FLIP_NONE);

    // Affichage du texte
    SDL_Rect titleRect = {700, 90, 500, 120};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &titleRect);  // Affiche "Space Miner"

    switch (bg_button_a_afficher) {
        case 1:
            SDL_RenderCopy(renderer, imageTextures[6][0], NULL, &bgButton1Rect);  // Affiche le rectangle derriere "Continue"
            break;
        case 2:
            SDL_RenderCopy(renderer, imageTextures[6][0], NULL, &bgButton2Rect);  // Affiche le rectangle derriere "New game"
            break;
        case 3:
            SDL_RenderCopy(renderer, imageTextures[6][0], NULL, &bgButton3Rect);  // Affiche le rectangle derriere "Settings"
            break;
        default:
            break;
    }
    
    SDL_Rect text1Rect = {880, 320, 150, 45};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &text1Rect);  // Affiche "Continue"

    SDL_Rect text2Rect = {880, 420, 150, 45};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &text2Rect);  // Affiche "New game"

    SDL_Rect text3Rect = {880, 520, 150, 45};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &text3Rect);  // Affiche "Settings"

    SDL_RenderPresent(renderer);
}



