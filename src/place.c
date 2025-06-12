#include "place.h"

#include <SDL2/SDL.h>
#include "config.h"


/*
(SDL_Rect){SCREEN_WIDTH * getEmp().x / getScaleDev(), SCREEN_HEIGHT * getEmp().y / getScaleDev(), SCREEN_WIDTH * getEmp().w / getScaleDev(), SCREEN_WIDTH * getEmp().h / getScaleDev()};

(SDL_Rect){SCREEN_WIDTH * getEmp().x / getScaleDev(), SCREEN_HEIGHT * getEmp().y / getScaleDev(), SCREEN_WIDTH * textureWidth * getEmp().w / getScaleDev(), SCREEN_WIDTH * textureHeight * getEmp().h / getScaleDev()};
*/

static SDL_Rect emplacement = {50, 50, 5, 5};
static int scaleDev = 100;  // Puissance de 10
static int sign = 1;

SDL_Rect getEmp(void) {
    return emplacement;
}

int getSign(void) {
    return sign;
}

int getScaleDev(void) {
    return scaleDev;
}

void setScaleDev(void) {
    if (scaleDev <= 1000 && sign == 1) {
        scaleDev *= 10;

        emplacement.x = emplacement.x * 10;
        emplacement.y = emplacement.y * 10;
        emplacement.w = emplacement.w * 10;
        emplacement.h = emplacement.h * 10;
    } else if (scaleDev >= 100 && sign == -1) {
        scaleDev /= 10;

        emplacement.x = emplacement.x / 10;
        emplacement.y = emplacement.y / 10;
        emplacement.w = emplacement.w / 10;
        emplacement.h = emplacement.h / 10;
    }

    printf("New scale: %d\n", scaleDev);

}

void setSign(){
    sign *= (-1);
    printf("Sign: %d \n", sign);
}

void modifEmp(int dx, int dy, int dw, int dh) {
    emplacement.x += sign * dx;
    emplacement.y += sign * dy;
    emplacement.w += sign * dw;
    emplacement.h += sign * dh;

    printf("x:%.4f ; y:%.4f ; w:%.4f ; h:%.4f\n", emplacement.x / (float)scaleDev, emplacement.y / (float)scaleDev, emplacement.w / (float)scaleDev, emplacement.h / (float)scaleDev);
}
