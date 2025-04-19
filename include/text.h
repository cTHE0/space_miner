#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
	char text[64];    // Texte a afficher
	SDL_Color color;  // Couleur du texte
	TTF_Font *font;	  // Police de caractere du texte
} TextToLoad;

void quit_SDL_ttf(TTF_Font *font);
TTF_Font* loadFonts(const char* path, int size);
SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* text);
SDL_Texture **loadTextTextures(SDL_Renderer *renderer);
void destroyTextTextures(SDL_Texture **textTextures);
void init_SDL_ttf(void);

extern TTF_Font *font;
extern SDL_Color black;

#endif