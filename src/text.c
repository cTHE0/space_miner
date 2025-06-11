#include "text.h"

#include <stdio.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"
#include "tools.h"
#include "renderer.h"
#include "assets_gestion.h"


void renderNumber(SDL_Renderer *renderer, SDL_Texture **textTextures, int numberDisplayed, SDL_Rect destRect) {
    int NbDigits = countDigits(numberDisplayed);

    char buffer[32] = {0};
    sprintf(buffer, "%d", numberDisplayed);

    for (int i = 0; i < NbDigits; i++) {
        char d = buffer[i];
        if (d >= '0' && d <= '9') {
            SDL_RenderCopy(renderer, textTextures[22 + (int)(d - '0')], NULL, &destRect);
            destRect.x += destRect.w;  // Avance vers la droite pour le chiffre suivant
        }
    }
}

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
                                            {"0", BLACK, fonts[0]},
                                            {"1", BLACK, fonts[0]},
                                            {"2", BLACK, fonts[0]},
                                            {"3", BLACK, fonts[0]},
                                            {"4", BLACK, fonts[0]},
                                            {"5", BLACK, fonts[0]},
                                            {"6", BLACK, fonts[0]},
                                            {"7", BLACK, fonts[0]},
                                            {"8", BLACK, fonts[0]},
                                            {"9", BLACK, fonts[0]},
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
                                            {"Manage builds", BLACK, fonts[0]},
                                            {"Overview of build", BLACK, fonts[0]},
                                            {"Building queue", BLACK, fonts[0]},
                                            {"Bunkers information", BLACK, fonts[0]},
                                            {"Container information", BLACK, fonts[0]},
                                            {"Abundance (%)", BLACK, fonts[0]},
                                            {"Mineral", BLACK, fonts[0]},
                                            {"NEW", RED, fonts[0]},
                                            {"New build", BLACK, fonts[0]},
                                            {"Upgrade", BLACK, fonts[0]},
                                            {"RESTART", BLACK, fonts[0]}
                                           };

    for (int i = 0; i < CST_TEXT_NUMBER; i++)
 {
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
}
