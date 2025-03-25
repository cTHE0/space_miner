#include "assets_gestion.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture **loadTextures(SDL_Renderer *renderer) {
    SDL_Texture **textures = malloc(PNG_IMAGE_NUMBER * sizeof(SDL_Texture*));
    char pngNames[PNG_IMAGE_NUMBER][128] = {"assets/img/ships/s1.png",  // 128 : limite de caractères par chaine de caractère
                                            "assets/img/ships/s2.png",
                                            "assets/img/ships/s3.png",
                                            "assets/img/planetes/p1.png",
                                            "assets/img/planetes/p2.png",
                                            "assets/img/planetes/p3.png",
                                            "assets/img/planetes/p4.png",
                                            "assets/img/planetes/p5.png",
                                            "assets/img/planetes/p6.png",
                                            "assets/img/planetes/p7.png",
                                            "assets/img/planetes/p8.png",
                                            "assets/img/planetes/p9.png",
                                            "assets/img/planetes/p10.png",
                                            "assets/img/maps/m1.png",
                                            "assets/img/others/add_button.png"};

    for (int i = 0; i < PNG_IMAGE_NUMBER; i++) {
        textures[i] = IMG_LoadTexture(renderer, pngNames[i]);
        if (!textures[i]) {
            printf("Erreur chargement de la texture %d (%s) : %s\n",i , pngNames[i], IMG_GetError());
            return NULL;
        }
    }
    return textures;
}

void SDL_DestroyTextures(SDL_Texture **textures) {
    for (int i = 0; i < PNG_IMAGE_NUMBER; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}
