#include "landing_page.h"
#include "config.h"
#include "text.h"
#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

LPFrameControler lp_frame_controller = {0, 0, 0, 0, 0};

SDL_Rect bgButton1Rect = {780, 320, 350, 55};
SDL_Rect bgButton2Rect = {780, 420, 350, 55};
SDL_Rect bgButton3Rect = {780, 520, 350, 55};

int bg_button_a_afficher;

void updateFrameIndex() {
    if (SDL_GetTicks() > lp_frame_controller.lastFrameTime1 + SHIP_FRAME_DELAY) {
        lp_frame_controller.frameIndex1 = (lp_frame_controller.frameIndex1 + 1) % 160; // 4 images dans le sprite sheet
        lp_frame_controller.lastFrameTime1 = SDL_GetTicks();
    }
    if (SDL_GetTicks() > lp_frame_controller.lastFrameTime2 + 0.14*SHIP_FRAME_DELAY) {
        lp_frame_controller.frameIndex2 = (lp_frame_controller.frameIndex2 + 1) % SCREEN_WIDTH;
        lp_frame_controller.frameIndex3 = (lp_frame_controller.frameIndex3 + 1) % SCREEN_HEIGHT;
        lp_frame_controller.lastFrameTime2 = SDL_GetTicks();
    }
}

// Affichage du menu principal
void afficherMenu(SDL_Renderer *renderer, SDL_Texture *map, SDL_Texture *planet, SDL_Texture *asteroid1, SDL_Texture *asteroid2, SDL_Texture *button_bg, SDL_Texture **cstTextTextures) {
    SDL_RenderClear(renderer);

    SDL_Rect srcRect = {lp_frame_controller.frameIndex1 * 100, 0, 100, 100}; // Frame actuelle sur le sprite sheet
    SDL_Rect destRect = {85, 198, 500, 500};       // Position et taille affichée
    SDL_Rect titleRect = {700, 90, 500, 120};
    SDL_Rect text1Rect = {880, 320, 150, 45};
    SDL_Rect text2Rect = {880, 420, 150, 45};
    SDL_Rect text3Rect = {880, 520, 150, 45};
    SDL_Rect asteroid1Rect = {SCREEN_WIDTH - 4*lp_frame_controller.frameIndex2, lp_frame_controller.frameIndex2, 50, 50};
    SDL_Rect asteroid2Rect = {80 + SCREEN_WIDTH - 4*lp_frame_controller.frameIndex2, 40 + lp_frame_controller.frameIndex2, 40, 40};
    SDL_Point asteroid1Center = {asteroid1Rect.w/2, asteroid1Rect.h/2};
    SDL_Point asteroid2Center = {asteroid2Rect.w/2, asteroid2Rect.h/2};


    SDL_RenderCopy(renderer, map, NULL, NULL);
    SDL_RenderCopyEx(renderer, asteroid1, NULL, &asteroid1Rect, lp_frame_controller.frameIndex2, &asteroid1Center, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, asteroid2, NULL, &asteroid2Rect, lp_frame_controller.frameIndex2, &asteroid2Center, SDL_FLIP_NONE);
    SDL_RenderCopy(renderer, planet, &srcRect, &destRect);
    SDL_RenderCopy(renderer, cstTextTextures[0], NULL, &titleRect);

    switch (bg_button_a_afficher)
    {
    case 1:
        SDL_RenderCopy(renderer, button_bg, NULL, &bgButton1Rect);
        break;
    
    case 2:
        SDL_RenderCopy(renderer, button_bg, NULL, &bgButton2Rect);
        break;
    
    case 3:
        SDL_RenderCopy(renderer, button_bg, NULL, &bgButton3Rect);
        break;
    
    default:
        break;
    }

    SDL_RenderCopy(renderer, cstTextTextures[1], NULL, &text1Rect);
    SDL_RenderCopy(renderer, cstTextTextures[2], NULL, &text2Rect);
    SDL_RenderCopy(renderer, cstTextTextures[3], NULL, &text3Rect);

    SDL_RenderPresent(renderer);
}

// Gère les événements du menu
void handleMenuEvents(GameState *state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        SDL_Point point = {x, y};
        switch (event.type)
        {
        case SDL_QUIT:
            *state = QUIT;
            break;
        
        case SDL_KEYDOWN:
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
            } break;
        
            case SDL_MOUSEMOTION:
                // Vérifier si la souris est dans les zones des boutons
                if (SDL_PointInRect(&point, &bgButton1Rect)) {
                    bg_button_a_afficher = 1;
                }
                else if (SDL_PointInRect(&point, &bgButton2Rect)) {
                    bg_button_a_afficher = 2;
                }
                else if (SDL_PointInRect(&point, &bgButton3Rect)) {
                    bg_button_a_afficher = 3;
                }
                else {
                    bg_button_a_afficher = 0;
                }
                break;
        }
    }
}
