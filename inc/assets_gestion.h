#ifndef ASSETS_GESTION_H
#define ASSETS_GESTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

SDL_Texture* IMG_LoadTextureWithAlpha(const char* filePath, Uint8 alpha);
SDL_Texture ***loadTextures(void);
	int foldersNbFunction(const char *path);
	char **foldersPathFunction(const char *path);
	int *filesNbFunction(char **foldersPath);
	int compare(const void *a, const void *b);
void initRects(SDL_Texture **textTextures);
void initSounds(Mix_Chunk ***sounds);
void destroyImageTextures(SDL_Texture ***imageTextures);
void freeSongs(Mix_Chunk **sounds);

#define BLACK (SDL_Color){0, 0, 0, 255}
#define WHITE (SDL_Color){255, 255, 255, 255}
#define RED (SDL_Color){255, 0, 0, 255}
#define RED2 (SDL_Color){218, 0, 0, 255}
#define GREEN (SDL_Color){0, 255, 0, 255}
#define GREEN2 (SDL_Color){59, 198, 0, 255}
#define BLUE (SDL_Color){0, 0, 255, 255}
#define ORANGE (SDL_Color){255, 95, 31, 255}
#define GOLD (SDL_Color){255, 200, 0, 255}
#define DARK_RED (SDL_Color){59, 0, 0, 255}

#endif
