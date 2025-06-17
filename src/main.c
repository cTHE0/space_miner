#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "planet.h"
#include "ship.h"
#include "camera.h"
#include "event.h"
#include "renderer.h"
#include "assets_gestion.h"
#include "config.h"
#include "landing_page.h"
#include "text.h"
#include "place.h"
#include "build.h"
#include "asteroid.h"


const uint32_t currentSeed = 2;  // Peut prendre des valeurs entre 1 et 2**32-1


int main(void) {
    // Initialisation 
    srand(time(NULL));
    short gameBegun = 0;

    SDL_Window *window;
    initSDL(&window);

    Ship *ships = NULL;
    int shipCount = INIT_SHIP_COUNT;

    Planet *planets = NULL;
    int planetCount = INIT_PLANET_COUNT;

    TTF_Font *fonts[FONT_NUMBER] = {0};
    loadFonts(fonts);

    SDL_Texture ***imageTextures = loadTextures();
    SDL_Texture **textTextures = loadTextTextures(fonts);

    loadNumbers(fonts[0]);

    initRects(textTextures);

    // Variables pour mesurer les FPS
    Uint32 toShowFPS = SDL_GetTicks();
    Uint32 toLimitFPS;
    int frameCount = 0;

    // Lancement de Void Reign
    GameState state = LANDING_PAGE;

    while (state != QUIT) {
        frameCount++;
        toLimitFPS = SDL_GetTicks();

        switch (state) {
            case LANDING_PAGE:
                handleMenuEvents(&state);
                updateFrameIndex();
                displayMenu(imageTextures, textTextures);

                if (state == GAME) {
                    gameBegun = 1;
                    initPlanets(&planets, planetCount);
                    initShips(&ships, shipCount, planets, planetCount);
                    initCamera(planets);
                    initAsteroids(planets, planetCount);
                }
                break;
            
            case GAME:
                initRects(textTextures);  // A SUPPRIMER, SEULEMENT POUR LE DEV
                handleEvents(textTextures, fonts, &state, ships, shipCount, planets, planetCount);
                updateShips(ships, shipCount);
                updatePlanets(planets, ships, shipCount, planetCount);
                updateBuilds(planets, planetCount);
                updateCameraFollow(ships, planets);
                updateAsteroids(planets,planetCount);
                updateTotalOre(planets, planetCount);
                displayGame(imageTextures, textTextures, ships, shipCount, planets, planetCount);
                break;
            
            default:
                state = QUIT;
                break;
        }

        // Affichage des FPS
        if (SDL_GetTicks() - toShowFPS >= 3000) {
            printf("FPS: %d\n", (frameCount * 1000) / (SDL_GetTicks() - toShowFPS));
            frameCount = 0;
            toShowFPS = SDL_GetTicks();
        }
            
        // Limite les FPS
        if ((SDL_GetTicks() - toLimitFPS) < 16) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - toLimitFPS));
        }
    }
    
    // Fermeture du programme
    if (gameBegun) {
        destroyShips(ships, shipCount);
        destroyPlanets(planets);
        freeAsteroid();
    }

    // POUR LE DEV 
    printf("(SDL_Rect){SCREEN_WIDTH * %.4f, SCREEN_HEIGHT * %.4f, SCREEN_WIDTH * %.4f, SCREEN_WIDTH * %.4f};\n", getEmp().x, getEmp().y, getEmp().w, getEmp().h);
    
    destroyImageTextures(imageTextures);
    destroyTextTextures(textTextures);
    destroyFonts(fonts);
    quitSDL(window);
    return 0;
}
