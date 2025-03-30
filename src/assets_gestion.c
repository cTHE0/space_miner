#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "assets_gestion.h"
#include "config.h"

SDL_Texture* IMG_LoadTextureWithAlpha(SDL_Renderer* renderer, const char* filePath, Uint8 alpha) {
    // Charger directement la texture avec SDL_Image
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        printf("Erreur lors du chargement de la texture : %s\n", IMG_GetError());
        return NULL;
    }

    // Définir le mode de mélange pour gérer l'alpha
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
        printf("Erreur lors du réglage du mode de mélange : %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }

    // Appliquer la transparence
    if (SDL_SetTextureAlphaMod(texture, alpha) != 0) {
        printf("Erreur lors de la modification de l'alpha : %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }

    return texture;
}

SDL_Texture **loadTextures(SDL_Renderer *renderer) { // ATTENTION: ne pas oublier de modifier PNG_IMAGE_NUMBER !!
    SDL_Texture **textures = malloc(PNG_IMAGE_NUMBER * sizeof(SDL_Texture*));
    char pngNames[PNG_IMAGE_NUMBER][128] = {"assets/img/ships/s1.png",  // 128 : limite de caractères par chaine de caractère
                                            "assets/img/ships/s2.png",
                                            "assets/img/ships/s3.png",
                                            "assets/img/planets/p1.png",
                                            "assets/img/planets/p2.png",
                                            "assets/img/planets/p3.png",
                                            "assets/img/planets/p4.png",
                                            "assets/img/planets/p5.png",
                                            "assets/img/planets/p6.png",
                                            "assets/img/planets/p7.png",
                                            "assets/img/planets/p8.png",
                                            "assets/img/planets/p9.png",
                                            "assets/img/planets/p10.png",
                                            "assets/img/maps/m1.png",
                                            "assets/img/others/add_button.png",
                                            "assets/img/maps/m2.png",
                                            "assets/img/maps/m3.png",
                                            "assets/img/planets/lp_planet.png",
                                            "assets/img/asteroids/asteroid1.png",
                                            "assets/img/asteroids/asteroid2.png",
                                            "assets/img/others/bg_button.png",
                                        };

    for (int i = 0; i < PNG_IMAGE_NUMBER; i++) {
        textures[i] = IMG_LoadTexture(renderer, pngNames[i]);
        if (!textures[i]) {
            printf("Erreur chargement de la texture %d (%s) : %s\n",i , pngNames[i], IMG_GetError());
            return NULL;
        }
    
    textures[20] = IMG_LoadTextureWithAlpha(renderer, "assets/img/others/bg_button.png", 100);
    }
    return textures;
}

void destroyImageTextures(SDL_Texture **imageTextures) {
    for (int i = 0; i < PNG_IMAGE_NUMBER; i++) {
        SDL_DestroyTexture(imageTextures[i]);
    }
    free(imageTextures);
}
