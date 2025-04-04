#ifndef SHIP_WINDOW_H
#define SHIP_WINDOW_H

#include "ship.h"


typedef struct
{
    Ship ship;
    SDL_Rect destRect;
    SDL_Rect crossRect;
} ShipWindow;

void afficheShipWindow(SDL_Renderer *renderer, SDL_Texture **imageTextures, SDL_Texture **textTextures);
void openWindowGestion(int x, int y, Ship *ships, int nb_ships);

extern which_ship_is_viewed;
#endif
