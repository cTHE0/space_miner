#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h> 
#include "config.h"

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
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
    }
    return font;
}

SDL_Texture* create_text_texture(SDL_Renderer* renderer, TTF_Font* font, int color[3], const char* text) {
    SDL_Color sdl_color = {color[0], color[1], color[2], 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, sdl_color);
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

SDL_Texture **loadTextTextures(SDL_Renderer *renderer) {
    TTF_Font *font = loadFonts("assets/fonts/f1.ttf", 50);
    SDL_Texture **textTextures = malloc(CST_TEXT_NUMBER * sizeof(SDL_Texture*));
    char *cstTexts[CST_TEXT_NUMBER] = {"Space Miner",
                                       "Continue",
                                       "New Game",
                                       "Settings"}; // ATENTION: ne pas oublier de modifier CST_TEXT_NUMBER


    for (int i = 0; i < CST_TEXT_NUMBER; i++) {
        textTextures[i] = create_text_texture(renderer, font, WHITE, cstTexts[i]);
    }

    return textTextures;
}

void destroyTextTextures(SDL_Texture **textTextures) {
    for (int i = 0; i<CST_TEXT_NUMBER; i++) {
        SDL_DestroyTexture(textTextures[i]);
    }
    free(textTextures);
}
