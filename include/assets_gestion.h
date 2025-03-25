#ifndef ASSETS_GESTION_H
#define ASSETS_GESTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture **loadTextures(SDL_Renderer *renderer);
void SDL_DestroyTextures(SDL_Texture **textures);

#endif
