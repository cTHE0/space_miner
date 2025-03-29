#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_bool init_SDL_ttf() {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return SDL_FALSE;
    }
    return SDL_TRUE;
}

void quit_SDL_ttf(TTF_Font *font) {
    TTF_CloseFont(font);
    TTF_Quit();
}

TTF_Font* load_font(const char* path, int size) {
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
