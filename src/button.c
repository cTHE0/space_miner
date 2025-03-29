#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "button.h"

// Charger un bouton avec une texture
Button createButton(SDL_Renderer *renderer, const char *texturePath, int x, int y, int w, int h) {
    Button button;
    button.texture = IMG_LoadTexture(renderer, texturePath);
    if (!button.texture) {
        printf("Erreur chargement de la texture du bouton : %s\n", IMG_GetError());
    }
    button.rect = (SDL_Rect){x, y, w, h};
    return button;
}

// Détruire un bouton
void destroyButton(Button *button) {
    if (button->texture) {
        SDL_DestroyTexture(button->texture);
        button->texture = NULL;
    }
}

// Rendre le bouton
void renderButton(SDL_Renderer *renderer, Button *button) {
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}
