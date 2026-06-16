#ifndef INFO_VIEW_H
#define INFO_VIEW_H

#include <SDL2/SDL.h>

SDL_Rect getSideBarButtonRect();
SDL_Rect getSettingsIconRect(void);
void initInfoViewRects(SDL_Texture **textTextures);
void displayInfoView(SDL_Texture ***imageTextures, SDL_Texture **textTextures);


#endif