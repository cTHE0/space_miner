#ifndef INFO_VIEW_H
#define INFO_VIEW_H

#include <SDL2/SDL.h>

SDL_Rect getSideBarButtonRect();
void initInfoViewRects(SDL_Texture **textTextures);
void displayInfoView(SDL_Texture ***imageTextures);


#endif