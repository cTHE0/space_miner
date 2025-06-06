#ifndef PLACE_H
#define PLACE_H

#include <SDL2/SDL.h>

SDL_Rect get_emp();

int get_sign();

void change_sign();

void modif_emp(int dx, int dy, int dw, int dh);

#endif