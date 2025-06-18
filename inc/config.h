#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1920 // Dimensions 16/9 courantes : 1280x720, 1920x1080
#define SCREEN_HEIGHT 1080
#define MAP_SIZE AREA_SIZE * NUMBER_OF_AREA_PER_WIDTH
#define AREA_SIZE 20000  // une parcelle = une image carree de background
#define NUMBER_OF_AREA_PER_WIDTH 10  // Nombre de parcelle par cote
#define SOLAR_SYSTEM_SIZE 16000  // Taille d'un cote (c'est un carre)

#define INIT_PLANET_COUNT 30
#define INIT_SHIP_COUNT 3
#define ORE_TYPE_COUNT 5
#define CST_TEXT_NUMBER 79
#define FONT_NUMBER 1
#define BUILD_TYPE_COUNT 12
#define ASTEROID_COUNT 500 // nombre d'astéroids par système solaire

#define SPRITE_SHEETS_DELAY 100  // En nombre de tick
#define WAIT_TIME_SHIP 1000  // En millisecondes
#define WAIT_TIME_PLANET 5000  // En millisecondes
#define REFRESH_TIME_BUILDS 1000  // En millisecondes
#define TANKS_UPDATE_INTERVAL 200  // En millisecondes
#define ASTEROID_TICK 300
#define ENEMY_GENERATION_PERIOD 10000

#define STEP_TRANSLATION 40
#define LIMIT_UNZOOM 0.05
#define SHIP_SPEED 10.f
#define FPS 60

extern const uint32_t currentSeed;

#endif
