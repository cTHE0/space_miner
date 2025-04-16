#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1920 // (autre 16/9 courant plus petit : 1280 par 720)
#define SCREEN_HEIGHT 1080
#define MAP_SIZE 84000


#define INIT_PLANET_COUNT 3
#define INIT_SHIP_COUNT 1
#define CST_TEXT_NUMBER 20


#define SPRITE_SHEETS_DELAY 100  // En nombre de tick
#define WAIT_TIME_SHIP 1000  // En millisecondes
#define FUEL_UPDATE_INTERVAL 200  // En millisecondes

#define STEP_TRANSLATION 40
#define SHIP_SPEED 0.2f

#define WHITE (int[3]){255, 255, 255}  // COULEURS
#define BLACK (int[3]){0  , 0  ,   0}

#define carre(x) (x) * (x)

#endif
