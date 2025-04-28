#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1920 // (autre 16/9 courant plus petit : 1280 par 720)
#define SCREEN_HEIGHT 1080
#define MAP_SIZE 350000

#define INIT_PLANET_COUNT 4
#define INIT_SHIP_COUNT 3
#define ORE_TYPE_COUNT 5
#define CST_TEXT_NUMBER 42
#define FONT_NUMBER 1

#define SPRITE_SHEETS_DELAY 100  // En nombre de tick
#define WAIT_TIME_SHIP 1000  // En millisecondes
#define TANKS_UPDATE_INTERVAL 200  // En millisecondes

#define STEP_TRANSLATION 40
#define SHIP_SPEED 2.f
#define FPS 60

extern const uint32_t currentSeed;

#endif
