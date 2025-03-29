#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_bool init_SDL_ttf();
void quit_SDL_ttf(TTF_Font *font);
TTF_Font* load_font(const char* path, int size);
SDL_Texture* create_text_texture(SDL_Renderer* renderer, TTF_Font* font, int color[3], const char* text);

#endif