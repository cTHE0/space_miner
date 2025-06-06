#include <SDL2/SDL.h>
#include "config.h"
/*
(SDL_Rect){SCREEN_WIDTH * get_emp().x /160, SCREEN_HEIGHT * get_emp().y/160, SCREEN_WIDTH * get_emp().w/160, SCREEN_WIDTH * get_emp().h/160};
*/

SDL_Rect emplacement = {0, 0, 0, 0};
int sign = 1;

SDL_Rect get_emp(){
    return emplacement;
}

int get_sign(){
    return sign;
}

void change_sign(){
    sign *= (-1);
    printf("Sign: %d \n", sign);
}

void modif_emp(int dx, int dy, int dw, int dh) {
    emplacement.x += sign*dx;
    emplacement.y += sign*dy;
    emplacement.w += sign*dw;
    emplacement.h += sign*dh;
    
    printf("x:%d ; y:%d ; w:%d ; h:%d \n", emplacement.x, emplacement.y, emplacement.w, emplacement.h);
}
