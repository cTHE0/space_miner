#ifndef PLACE_H
#define PLACE_H

#include <SDL2/SDL.h>

SDL_Rect get_emp(void);
int get_sign(void);
void change_sign(void);
void modif_emp(int dx, int dy, int dw, int dh);

#endif