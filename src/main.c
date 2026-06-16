#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>
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
#include "enemy.h"
#include "tile.h"
#include "window.h"
#include "settings.h"
#include "notify.h"
#include "meta.h"
#include "events.h"
#include "objectives.h"
#include "threat.h"


const uint32_t currentSeed = 1;  // Peut prendre des valeurs entre 1 et 2**32-1


int main(void) {
    // Initialisation 
    srand(time(NULL));
    short gameBegun = 0;

    SDL_Window *window;
    initSDL(&window);

    Mix_Chunk **sounds;
    initSounds(&sounds);
    initSettings();
    initMeta();
    initEvents();
    initThreat();

    Ship *ships = NULL;
    int shipCount = INIT_SHIP_COUNT;

    Planet *planets = NULL;
    int planetCount = INIT_PLANET_COUNT;

    TTF_Font *fonts[FONT_NUMBER] = {0};
    loadFonts(fonts);

    SDL_Texture ***imageTextures = loadTextures();
    SDL_Texture **textTextures = loadTextTextures(fonts);

    loadNumbers(fonts[0]);
    initNotify(fonts[0]);
    initObjectives(fonts[0]);

    initRects(textTextures);

    // Variables pour mesurer les FPS
    Uint32 toShowFPS = SDL_GetTicks();
    Uint32 toLimitFPS;
    int frameCount = 0;

    // Lancement de Void Reign
    GameState gameState = LANDING_PAGE;
    Mix_PlayChannel(0, sounds[0], -1);

    while (gameState != QUIT) {
        frameCount++;
        toLimitFPS = SDL_GetTicks();

        switch (gameState) {
            case LANDING_PAGE:
                handleMenuEvents(sounds, &gameState, &gameBegun, &ships, &shipCount, &planets, &planetCount);
                updateFrameIndex();
                displayMenu(imageTextures, textTextures);
                break;
            
            case GAME:
                handleEvents(textTextures, fonts, sounds, &gameState, &ships, &shipCount, planets, planetCount);
                updateGame(textTextures, fonts, &ships, &shipCount, planets, planetCount, sounds);
                displayGame(imageTextures, textTextures, ships, shipCount, planets, planetCount);
                break;
            
            default:
                gameState = QUIT;
                break;
        }

        // Mesure des FPS (affichage uniquement en mode debug)
        if (SDL_GetTicks() - toShowFPS >= 3000) {
#ifdef DEBUG_FPS
            printf("FPS: %d\n", (frameCount * 1000) / (SDL_GetTicks() - toShowFPS));
#endif
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
        destroyShips(ships);
        destroyPlanets(planets);
        destroyAsteroids();
        destroyTiles();
    }

    destroyNotify();
    destroyObjectives();
    destroyImageTextures(imageTextures);
    destroyTextTextures(textTextures);
    destroyFonts(fonts);
    freeSongs(sounds);
    quitSDL(window);
    return 0;
}
