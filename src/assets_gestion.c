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

SDL_Texture ***loadTextures(SDL_Renderer *renderer) {
    // Ordre de stockage dans imageTextures : 
    // ships(0), planetes(1), maps(2), attackers(3), buttons(4), asteroids(5), others(6)

    // Parametre du tableau de textures (A MODIFIER)
    int nbCategories = 7;
    int nbPicturePerCategories[7] = {10, 11, 3, 1, 2, 2, 2};

    // Chargement des noms des fichiers (A MODIFIER)
    char pngNames[31][128] = {"assets/img/ships/1.png",  // 128 : limite de caractères par chaine de caractère
                              "assets/img/ships/2.png",
                              "assets/img/ships/3.png",
                              "assets/img/ships/4.png",
                              "assets/img/ships/5.png",
                              "assets/img/ships/6.png",
                              "assets/img/ships/7.png",
                              "assets/img/ships/8.png",
                              "assets/img/ships/9.png",
                              "assets/img/ships/10.png",
                              "assets/img/planets/1.png",
                              "assets/img/planets/2.png",
                              "assets/img/planets/3.png",
                              "assets/img/planets/4.png",
                              "assets/img/planets/5.png",
                              "assets/img/planets/6.png",
                              "assets/img/planets/7.png",
                              "assets/img/planets/8.png",
                              "assets/img/planets/9.png",
                              "assets/img/planets/10.png",
                              "assets/img/planets/11.png",
                              "assets/img/maps/1.png",
                              "assets/img/maps/2.png",
                              "assets/img/maps/3.png",
                              "assets/img/attackers/1.png",
                              "assets/img/buttons/add_button.png",
                              "assets/img/buttons/cross.png",
                              "assets/img/asteroids/1.png",
                              "assets/img/asteroids/2.png",
                              "assets/img/others/bg_button.png",
                              "assets/img/others/bg_window.png",
                             };

    // Allocation et remplissage du tableau de textures
    SDL_Texture ***imageTextures = malloc(nbCategories * sizeof(SDL_Texture**));
    int shift = 0;
    for (int i = 0; i < nbCategories - 1; i++) {
        imageTextures[i] = malloc(nbPicturePerCategories[i] * sizeof(SDL_Texture*));

        for (int j = 0; j < nbPicturePerCategories[i]; j++) {
            imageTextures[i][j] = IMG_LoadTexture(renderer, pngNames[shift + j]);
            if (!imageTextures[i][j]) {
                printf("Erreur chargement de la texture %s : %s\n", pngNames[j], IMG_GetError());
                return NULL;
            }
        }
        shift += nbPicturePerCategories[i];
    }

    // Allocation de la sous-liste 'others' a la main ('others' est tjr a la fin du tableau de texture)
    imageTextures[nbCategories - 1] = malloc(nbPicturePerCategories[nbCategories - 1] * sizeof(SDL_Texture*));
    imageTextures[nbCategories - 1][0] = IMG_LoadTextureWithAlpha(renderer, pngNames[shift + 0], 100);
    imageTextures[nbCategories - 1][1] = IMG_LoadTextureWithAlpha(renderer, pngNames[shift + 1], 230);
    
    return imageTextures;
}

void destroyImageTextures(SDL_Texture ***imageTextures) {
    int nbCategories = 7;
    int nbPicturePerCategories[7] = {10, 11, 3, 1, 2, 2, 2};

    for (int i = 0; i < nbCategories; i++) {
        for (int j = 0; j < nbPicturePerCategories[i]; j++) {
            SDL_DestroyTexture(imageTextures[i][j]);
        }
        free(imageTextures[i]);
    }
    free(imageTextures);
}