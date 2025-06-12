#ifndef PLACE_H
#define PLACE_H

#include <SDL2/SDL.h>

SDL_Rect getEmp(void);
int getSign(void);
void setSign(void);
void modifEmp(int dx, int dy, int dw, int dh);
int getScaleDev(void);
void setScaleDev(void);

#endif