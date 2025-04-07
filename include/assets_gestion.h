#ifndef ASSETS_GESTION_H
#define ASSETS_GESTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"

SDL_Texture* IMG_LoadTextureWithAlpha(SDL_Renderer* renderer, const char* filePath, Uint8 alpha);
SDL_Texture ***loadTextures(SDL_Renderer *renderer);
	int foldersNbFunction(const char *path);
	char **foldersPathFunction(const char *path);
	int *filesNbFunction(char **foldersPath);
	int compare(const void *a, const void *b);
void destroyImageTextures(SDL_Texture ***textures);

#endif
