#include "text.h"

#include <stdio.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"
#include "renderer.h"
#include "assets_gestion.h"

/* ------------------------------------------GESTION NOMBRES-------------------------------------------------------*/

SDL_Texture *number_textures[10]; // textures pour les chiffres de 0 à 9

// Préparer les textures pour les chiffres de 0 à 9
void load_numbers(TTF_Font *font) {
    char digit_str[2] = "0";
    for (int i = 0; i < 10; i++) {
        digit_str[0] = '0' + i;
        number_textures[i] = createTextTexture(font, (SDL_Color){16, 16, 16, 255}, digit_str);
    }
}

void render_number(SDL_Renderer *renderer, int i, SDL_Rect *dstRect, int nb_chiffres) {
    if (nb_chiffres <= 0 || nb_chiffres > 20) return; // Limite de sécurité

    char buffer[21]; // Assez grand pour nb_chiffres jusqu'à 20 + '\0'
    snprintf(buffer, sizeof(buffer), "%0*d", nb_chiffres, i); // Formatage avec zéros à gauche

    SDL_Rect rect = *dstRect;

    for (int j = 0; j < nb_chiffres; j++) {
        char c = buffer[j];
        if (c >= '0' && c <= '9') {
            int digit = c - '0';
            SDL_RenderCopy(renderer, number_textures[digit], NULL, &rect);
            rect.x += rect.w; // Avance vers la droite pour le chiffre suivant
        }
    }
}

/* ------------------------------------------GESTION NOMBRES-------------------------------------------------------*/

void loadFonts(TTF_Font **fonts) {  // ATTENTION: ne pas oublier de modifier FONT_NUMBER
    fonts[0] = loadFont("assets/fonts/f1.ttf", 56);  // Taille optimale pour cette police : 56
}

TTF_Font* loadFont(const char* path, int size) {
    TTF_Font *newFont = TTF_OpenFont(path, size);

    if (!newFont) {
        fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
        return NULL;
    }

    return newFont;
}

SDL_Texture **loadTextTextures(TTF_Font **fonts) {  // ATTENTION: ne pas oublier de modifier CST_TEXT_NUMBER
    SDL_Texture **textTextures = malloc(CST_TEXT_NUMBER * sizeof(SDL_Texture*));
    TextToLoad cstTexts[CST_TEXT_NUMBER] = {{"VOID REIGN:", WHITE, fonts[0]},
                                            {"Continue", WHITE, fonts[0]},
                                            {"New game", WHITE, fonts[0]},
                                            {"Settings", WHITE, fonts[0]},
                                            {"METS_CE_QUE_TU_VEUX_DANS_CET_EMPLACEMENT", BLACK, fonts[0]},
                                            {"Travel information", BLACK, fonts[0]},
                                            {"NOT ENOUGH FUEL", BLACK, fonts[0]},
                                            {"STOP", BLACK, fonts[0]},
                                            {"Tank manager", BLACK, fonts[0]},
                                            {"Base:", BLACK, fonts[0]},
                                            {"Target:", BLACK, fonts[0]},
                                            {"Ship condition", BLACK, fonts[0]},
                                            {"Repair", BLACK, fonts[0]},
                                            {"Reported problems", BLACK, fonts[0]},
                                            {"Tank composition", BLACK, fonts[0]},
                                            {"Tank 1", BLACK, fonts[0]},
                                            {"Tank 2", BLACK, fonts[0]},
                                            {"Tank 3", BLACK, fonts[0]},
                                            {"Tank 4", BLACK, fonts[0]},
                                            {"Tank 5", BLACK, fonts[0]},
                                            {"Tank 6", BLACK, fonts[0]},
                                            {"Tank 7", BLACK, fonts[0]},
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamicNumber", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Zone de stockage pour la texture d'un string
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"dynamiqueString", BLACK, fonts[0]},  // Idem
                                            {"THE MINERALS WAR", WHITE, fonts[0]},
                                            {"STOP", BLACK, fonts[0]},
                                            {"NOT ENOUGH FUEL!", RED, fonts[0]},
                                            {"Select a new base", ORANGE, fonts[0]},
                                            {"Select a new target", ORANGE, fonts[0]},
                                            {"Main info.", BLACK, fonts[0]},
                                            {"I  ", BLACK, fonts[0]},
                                            {"II ", BLACK, fonts[0]},
                                            {"III", BLACK, fonts[0]},
                                            {"IV", BLACK, fonts[0]},
                                            {"General information", BLACK, fonts[0]},
                                            {"Mineral abundance", BLACK, fonts[0]},
                                            {"Overview of buildings", BLACK, fonts[0]},
                                            {"Make new buildings", BLACK, fonts[0]},
                                            {"Builing queue", BLACK, fonts[0]}
                                           };

    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        textTextures[i] = createTextTexture(cstTexts[i].font, cstTexts[i].color, cstTexts[i].text);
    }

    //On charge textures des 10 chiffres
    load_numbers(fonts[0]);

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
        fprintf(stderr, "Erreur de creation de texture: %s\n", SDL_GetError());
    }
    return texture;
}

void updateTextTexture(SDL_Texture **updatedTextTexture, TextToLoad newText) {
    // Detruire l'ancienne texture
    if (*updatedTextTexture) {
        SDL_DestroyTexture(*updatedTextTexture);
    }

    // Creer et assigner la nouvelle texture
    *updatedTextTexture = createTextTexture(newText.font, newText.color, newText.text);
    if (!*updatedTextTexture) {
        fprintf(stderr, "Echec de la mise a jour de la texture du nouveau texte '%s'\n", newText.text);
    }
}

void destroyTextTextures(SDL_Texture **textTextures) {
    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        SDL_DestroyTexture(textTextures[i]);
    }
    free(textTextures);
}

void destroyFonts(TTF_Font **fonts) {
    for (int i = 0; i < FONT_NUMBER; i++) {
        TTF_CloseFont(fonts[i]);
    }
    for (int i = 0; i < 10; i++) {
        if (number_textures[i] != NULL) {
            SDL_DestroyTexture(number_textures[i]);
            number_textures[i] = NULL;
        }
    }

    if (TTF_WasInit()) {
        TTF_Quit();
    }
}
