#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void quit_SDL_ttf(TTF_Font *font);
TTF_Font* loadFonts(const char* path, int size);
SDL_Texture* create_text_texture(SDL_Renderer* renderer, TTF_Font* font, int color[3], const char* text);
SDL_Texture **loadTextTextures(SDL_Renderer *renderer);
void destroyTextTextures(SDL_Texture **textTextures);
void init_SDL_ttf(void);

#endif