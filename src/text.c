#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h> 
#include "config.h"
#include "text.h"
#include "renderer.h"
#include "assets_gestion.h"


SDL_Texture **textTextures = NULL;
TTF_Font *uploadFont = NULL;


void loadFonts(void) {  // Utile pour un potentiel futur tableau de polices
    uploadFont = loadFont("assets/fonts/f1.ttf", 56);  // Taille optimale pour cette police : 56
}

TTF_Font* loadFont(const char* path, int size) {
    TTF_Font* newFont = TTF_OpenFont(path, size);

    if (!newFont) {
        fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
        return NULL;
    }

    return newFont;
}

SDL_Texture **loadTextTextures(void) {  // ATTENTION: ne pas oublier de modifier CST_TEXT_NUMBER
    SDL_Texture **textTextures = malloc(CST_TEXT_NUMBER * sizeof(SDL_Texture*));
    TextToLoad cstTexts[CST_TEXT_NUMBER] = {{"Space Miner", WHITE, uploadFont},
                                            {"Continue", WHITE, uploadFont},
                                            {"New game", WHITE, uploadFont},
                                            {"Settings", WHITE, uploadFont},
                                            {"Ship #1,887 – Basic Rocket Model S", BLACK, uploadFont},
                                            {"Travel information", BLACK, uploadFont},
                                            {"NOT ENOUGH FUEL", BLACK, uploadFont},
                                            {"STOP", BLACK, uploadFont},
                                            {"Tank manager", BLACK, uploadFont},
                                            {"Base:", BLACK, uploadFont},
                                            {"Target:", BLACK, uploadFont},
                                            {"Ship condition", BLACK, uploadFont},
                                            {"Repair", BLACK, uploadFont},
                                            {"Reported problems", BLACK, uploadFont},
                                            {"Tank composition", BLACK, uploadFont},
                                            {"Tank 1", BLACK, uploadFont},
                                            {"Tank 2", BLACK, uploadFont},
                                            {"Tank 3", BLACK, uploadFont},
                                            {"dynamicNumber", BLACK, uploadFont},  // Zone de stockage pour la texture d'un nombre variable
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamicNumber", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Zone de stockage pour la texture d'un string
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                            {"dynamiqueString", BLACK, uploadFont},  // Idem
                                           };

    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        textTextures[i] = createTextTexture(cstTexts[i].font, cstTexts[i].color, cstTexts[i].text);
    }

    return textTextures;
}

SDL_Texture* createTextTexture(TTF_Font* font, SDL_Color color, const char* text) {
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "Erreur de rendu du texte: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        fprintf(stderr, "Erreur de création de texture: %s\n", SDL_GetError());
    }
    return texture;
}

void updateTextTexture(SDL_Texture **updatedTextTexture, TextToLoad newText) {
    // Détruire l'ancienne texture
    if (*updatedTextTexture) {
        SDL_DestroyTexture(*updatedTextTexture);
    }

    // Créer et assigner la nouvelle texture
    *updatedTextTexture = createTextTexture(newText.font, newText.color, newText.text);
    if (!*updatedTextTexture) {
        fprintf(stderr, "Échec de la mise à jour de la texture du nouveau texte '%s'\n", newText.text);
    }
}

void destroyTextTextures(SDL_Texture **textTextures) {
    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        SDL_DestroyTexture(textTextures[i]);
    }
    free(textTextures);
}
