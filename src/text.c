#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h> 
#include "config.h"
#include "text.h"

TTF_Font *font;

SDL_Color black = {0, 0, 0, 255}; //noir opaque

void init_SDL_ttf(void) {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return;
    }
}

void quit_SDL_ttf(TTF_Font *font) {  // Permet de quitter SDL_ttf et de fermer le futur tableau des polices
    TTF_CloseFont(font);
    TTF_Quit();
}

TTF_Font* loadFonts(const char* path, int size) {  // Chargement de toutes les polices utilisees dans le jeu
    TTF_Font* nv_font = TTF_OpenFont(path, size);

    if (!nv_font) {
        fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
    }

    return nv_font;
}

SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* text) {
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

SDL_Texture **loadTextTextures(SDL_Renderer *renderer) {  // ATENTION: ne pas oublier de modifier CST_TEXT_NUMBER
    font = loadFonts("assets/fonts/f1.ttf", 56);  // Taille optimale pour cette police : 56
    SDL_Texture **textTextures = malloc(CST_TEXT_NUMBER * sizeof(SDL_Texture*));
    TextToLoad cstTexts[CST_TEXT_NUMBER] = {{"Space Miner", {255, 255, 255, 255}, font},
                                            {"Continue", {255, 255, 255, 255}, font},
                                            {"New game", {255, 255, 255, 255}, font},
                                            {"Settings", {255, 255, 255, 255}, font},
                                            {"Ship #1,887 – Basic Rocket Model S", {0, 0, 0, 255}, font},
                                            {"Travel information", {0, 0, 0, 255}, font},
                                            {"NOT ENOUGH FUEL", {0, 0, 0, 255}, font},
                                            {"STOP", {0, 0, 0, 255}, font},
                                            {"Tank manager", {0, 0, 0, 255}, font},
                                            {"Base:", {0, 0, 0, 255}, font},
                                            {"Target:", {0, 0, 0, 255}, font},
                                            {"Ship condition", {0, 0, 0, 255}, font},
                                            {"Repair", {0, 0, 0, 255}, font},
                                            {"Reported problems", {0, 0, 0, 255}, font},
                                            {"Tank composition", {0, 0, 0, 255}, font},
                                            {"Tank 1", {0, 0, 0, 255}, font},
                                            {"Tank 2", {0, 0, 0, 255}, font},
                                            {"Tank 3", {0, 0, 0, 255}, font},
                                            {"number", {0, 0, 0, 255}, font}, //Zone de stockage pour un numéro
                                            {"number", {0, 0, 0, 255}, font}, //Zone de stockage pour un autre numéro
                                            {"planet_name", {0, 0, 0, 255}, font}, //Texture qui contiendra le nom d'une planète (actualisé seulement lors de l'ouverture d'une fenêtre)
                                            {"planet_name", {0, 0, 0, 255}, font},
                                           };

    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        textTextures[i] = createTextTexture(renderer, cstTexts[i].font, cstTexts[i].color, cstTexts[i].text);
    }

    return textTextures;
}

void destroyTextTextures(SDL_Texture **textTextures) {
    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        SDL_DestroyTexture(textTextures[i]);
    }
    free(textTextures);
}
