#include "assets_gestion.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *loadSpriteSheet(SDL_Renderer *renderer, char address[]) {
    SDL_Texture *spriteSheet = NULL;
    spriteSheet = IMG_LoadTexture(renderer, "../assets/img/spritesheet.png");
    if (!spriteSheet) {
        printf("Erreur chargement sprite sheet : %s\n", IMG_GetError());
        return NULL;
    }
    return spriteSheet;
}
