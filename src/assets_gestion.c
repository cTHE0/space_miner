#include "assets_gestion.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

char *pngNames[] = {
    "attaquant",
    "bouton_ajouter",
    "fusee",
    "spritesheet",
    "planetes/p1",
    "planetes/p2",
    "planetes/p3",
    "planetes/p4",
    "planetes/p5",
    "planetes/p6",
    "planetes/p7",
    "planetes/p8",
    "planetes/p9",
    "planetes/p10",
}

SDL_Texture **loadTextures(SDL_Renderer *renderer, char address[]) {
    SDL_Texture **textures = malloc( PNG_IMAGE_NUMBER * sizeof(SDL_Texture*));
    SDL_Texture *spriteSheet = NULL;
    spriteSheet = IMG_LoadTexture(renderer, address);
    if (!spriteSheet) {
        printf("Erreur chargement sprite sheet : %s\n", IMG_GetError());
        return NULL;
    }
    return textures;
};
