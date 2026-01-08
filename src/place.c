#include "place.h"

#include <SDL2/SDL.h>
#include "config.h"
#include "math.h"


/*
Si l'on utilise place.c, decommenter la ligne 73 du main.

(SDL_Rect){SCREEN_WIDTH * getEmp().x, SCREEN_HEIGHT * getEmp().y, SCREEN_WIDTH * getEmp().w, SCREEN_WIDTH * getEmp().h};

(SDL_Rect){SCREEN_WIDTH * getEmp().x / getScaleDev(), SCREEN_HEIGHT * getEmp().y / getScaleDev(), SCREEN_WIDTH * textureWidth * getEmp().w / getScaleDev(), SCREEN_WIDTH * textureHeight * getEmp().h / getScaleDev()};
*/

static SDL_FRect emplacement = {0.2, 0.2, 0.05, 0.05};
static float scaleDev = 0.01;  // Puissance de 10 (0.001, 0.01, 0.1, 1, 10, 100, ...)
static int sign = 1;

SDL_FRect getEmp(void) {
    return emplacement;
}

int getSign(void) {
    return sign;
}

int getScaleDev(void) {
    return scaleDev;
}

void setScaleDev(void) {
    if (scaleDev <= 0.01 && sign == 1) {
        scaleDev *= 10;
    } else if (scaleDev >= 0.0001 && sign == -1) {
        scaleDev /= 10;
    }

    printf("New scale: %.4f\n", scaleDev);

}

void setSign(){
    sign *= (-1);
    printf("Sign: %d \n", sign);
}

void modifEmp(int dx, int dy, int dw, int dh) {
    emplacement.x += sign * scaleDev * dx;
    emplacement.y += sign *scaleDev * dy;
    emplacement.w += sign * scaleDev *dw;
    emplacement.h += sign *scaleDev * dh;

    printf("x:%.4f ; y:%.4f ; w:%.4f ; h:%.4f\n", emplacement.x, emplacement.y, emplacement.w, emplacement.h);
}
