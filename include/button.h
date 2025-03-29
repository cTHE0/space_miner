#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

// Structure du bouton
typedef struct {
    SDL_Texture *texture;
    SDL_Rect rect;
} Button;

// Fonctions de gestion des boutons
Button createButton(SDL_Renderer *renderer, const char *texturePath, int x, int y, int w, int h);
void destroyButton(Button *button);
void renderButton(SDL_Renderer *renderer, Button *button);

#endif
