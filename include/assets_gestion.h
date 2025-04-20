#ifndef ASSETS_GESTION_H
#define ASSETS_GESTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"

SDL_Texture* IMG_LoadTextureWithAlpha(const char* filePath, Uint8 alpha);
SDL_Texture ***loadTextures(void);
	int foldersNbFunction(const char *path);
	char **foldersPathFunction(const char *path);
	int *filesNbFunction(char **foldersPath);
	int compare(const void *a, const void *b);
void destroyImageTextures(SDL_Texture ***textures);

#define BLACK (SDL_Color){0, 0, 0, 255}
#define WHITE (SDL_Color){255, 255, 255, 255}
#define RED (SDL_Color){255, 0, 0, 255}
#define GREEN (SDL_Color){0, 255, 0, 255}
#define BLUE (SDL_Color){0, 0, 255, 255}

#endif
