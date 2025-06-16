#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Texture **textTextures;

typedef struct {
	char text[128];    // Texte a afficher
	SDL_Color color;  // Couleur du texte
	TTF_Font *font;	  // Police de caractere du texte
} TextToLoad;

void renderNumber(SDL_Renderer *renderer, int i, SDL_Rect dstRect);

void loadFonts(TTF_Font **fonts);
	TTF_Font* loadFont(const char* path, int size);

SDL_Texture* createTextTexture(TTF_Font* font, SDL_Color color, const char* text);
SDL_Texture **loadTextTextures(TTF_Font **fonts);
void updateTextTexture(SDL_Texture **updatedTextTexture, TextToLoad newText);
void destroyTextTextures(SDL_Texture **textTextures);
void destroyFonts(TTF_Font **fonts);

#endif