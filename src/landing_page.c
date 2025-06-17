#include "landing_page.h"

#include <SDL2/SDL.h>
#include "config.h"
#include "renderer.h"


static int bg_button_a_afficher = 0;

static lpFrameControler lpFrameController = {0, 0, 0, 0};

static SDL_Rect bgButton1Rect,
                bgButton2Rect,
                bgButton3Rect;


void initLandingPageRects(void) {
    bgButton1Rect = (SDL_Rect){SCREEN_WIDTH * 0.53, SCREEN_HEIGHT * 0.458, SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.07};
    bgButton2Rect = (SDL_Rect){SCREEN_WIDTH * 0.53, SCREEN_HEIGHT * 0.578, SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.07};
    bgButton3Rect = (SDL_Rect){SCREEN_WIDTH * 0.53, SCREEN_HEIGHT * 0.698, SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.07};
}


void handleMenuEvents(GameState *state) {   // Gere les evenements du menu
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
                    case SDLK_TAB:
                        bg_button_a_afficher = bg_button_a_afficher % 3 + 1;
                        break;
                    case SDLK_RETURN:
                        if (bg_button_a_afficher == 1 || bg_button_a_afficher == 2){
                            *state = GAME;
                        }
                        break;
                    default:
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                // Verifier si la souris est dans les zones des boutons
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
                    switch (bg_button_a_afficher) {
                        case 1:
                        case 2:
                            *state = GAME;
                            break;
                        default:
                            break;
                    }
                }
                break;

            default:
                break;
        }
    }
}

void updateFrameIndex(void) {
    if (SDL_GetTicks() > lpFrameController.lastFrameTime1 + SPRITE_SHEETS_DELAY) {
        lpFrameController.frameIndex1 = (lpFrameController.frameIndex1 + 1) % 160;  // 160 images dans le sprite sheet de la
        lpFrameController.lastFrameTime1 = SDL_GetTicks();                            // planetes qui tourne dans le menu
    }
    if (SDL_GetTicks() > lpFrameController.lastFrameTime2 + 0.15 * SPRITE_SHEETS_DELAY) {
        lpFrameController.frameIndex2 = (lpFrameController.frameIndex2 + 1) % (3000);
        lpFrameController.lastFrameTime2 = SDL_GetTicks();
    }
}

void displayMenu(SDL_Texture ***imageTextures, SDL_Texture **textTextures) {
    initLandingPageRects();
    
    // Initialisation graphique du menu
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imageTextures[5][2], NULL, NULL);

    // Affichage de la planetes en rotation
    SDL_Rect srcRect = {lpFrameController.frameIndex1 * 100, 0, 100, 100}; // Frame actuelle sur le sprite sheet de la planete du menu
    SDL_Rect destRect = {SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.2, SCREEN_WIDTH * 0.35, SCREEN_WIDTH * 0.35};  // Position et taille affichee
    SDL_RenderCopy(renderer, imageTextures[6][0], &srcRect, &destRect);

    // Affichages de l'asteroide 1
    SDL_Rect asteroid1Rect = {(0.0005 * lpFrameController.frameIndex2 - 0.1) * SCREEN_WIDTH, 0.0001 * lpFrameController.frameIndex2 * SCREEN_HEIGHT, SCREEN_WIDTH * 0.1, SCREEN_WIDTH * 0.1};

    SDL_RenderCopyEx(renderer, imageTextures[0][0], NULL, &asteroid1Rect, lpFrameController.frameIndex2, NULL, SDL_FLIP_NONE);

    // Affichages de l'asteroide 2
    SDL_Rect asteroid2Rect = {(0.001 * lpFrameController.frameIndex2 - 1) * SCREEN_WIDTH, (0.0012 * lpFrameController.frameIndex2 - 1.5) * SCREEN_HEIGHT, SCREEN_WIDTH * 0.05, SCREEN_WIDTH * 0.05};

    SDL_RenderCopyEx(renderer, imageTextures[0][1], NULL, &asteroid2Rect, -3 * lpFrameController.frameIndex2, NULL, SDL_FLIP_NONE);

    // Affichage du titre "Void Reign:"
    int textureWidth, textureHeight;  // Permet de garder les proportions du texte
    SDL_QueryTexture(textTextures[0], NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect titleRect = {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.07, textureWidth * SCREEN_HEIGHT * 0.003, textureHeight * SCREEN_HEIGHT * 0.003};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &titleRect);

    // Affichage du titre "The Minerals War"
    SDL_QueryTexture(textTextures[42], NULL, NULL, &textureWidth, &textureHeight);  // Permet de garder les proportions du texte

    SDL_Rect title2Rect = {SCREEN_WIDTH * 0.39, SCREEN_HEIGHT * 0.2, textureWidth * SCREEN_HEIGHT * 0.003, textureHeight * SCREEN_HEIGHT * 0.003};
    SDL_RenderCopy(renderer, textTextures[42], NULL, &title2Rect);

    // Affiche les bouton New game, Continue, Settings
    SDL_SetTextureBlendMode(imageTextures[5][1], SDL_BLENDMODE_BLEND);
    switch (bg_button_a_afficher) {
        case 1:
            SDL_RenderCopy(renderer, imageTextures[5][1], NULL, &bgButton1Rect);  // Affiche le rectangle derriere "Continue"
            break;
        case 2:
            SDL_RenderCopy(renderer, imageTextures[5][1], NULL, &bgButton2Rect);  // Affiche le rectangle derriere "New game"
            break;
        case 3:
            SDL_RenderCopy(renderer, imageTextures[5][1], NULL, &bgButton3Rect);  // Affiche le rectangle derriere "Settings"
            break;
        default:
            break;
    }
    SDL_SetTextureBlendMode(imageTextures[5][1], SDL_BLENDMODE_BLEND);
    SDL_QueryTexture(textTextures[1], NULL, NULL, &textureWidth, &textureHeight);  // Permet de garder les proportions du texte
    SDL_Rect text1Rect = {SCREEN_WIDTH * 0.618, SCREEN_HEIGHT * 0.45, textureWidth * SCREEN_HEIGHT * 0.0013, textureHeight * SCREEN_HEIGHT * 0.0013};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &text1Rect);  // Affiche "Continue"

    SDL_QueryTexture(textTextures[2], NULL, NULL, &textureWidth, &textureHeight);
    SDL_Rect text2Rect = {SCREEN_WIDTH * 0.608, SCREEN_HEIGHT * 0.57, textureWidth * SCREEN_HEIGHT * 0.0013, textureHeight * SCREEN_HEIGHT * 0.0013};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &text2Rect);  // Affiche "New game"

    SDL_QueryTexture(textTextures[3], NULL, NULL, &textureWidth, &textureHeight);
    SDL_Rect text3Rect = {SCREEN_WIDTH * 0.62, SCREEN_HEIGHT * 0.69, textureWidth * SCREEN_HEIGHT * 0.0013, textureHeight * SCREEN_HEIGHT * 0.0013};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &text3Rect);  // Affiche "Settings"

    SDL_RenderPresent(renderer);
}
