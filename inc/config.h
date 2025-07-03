#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>


#define SCREEN_WIDTH 1920 // Dimensions 16/9 courantes : 1280x720, 1920x1080
#define SCREEN_HEIGHT 1080
#define MAP_SIZE AREA_SIZE * NUMBER_OF_AREA_PER_WIDTH
#define AREA_SIZE 20000  // une parcelle = une image carree de background
#define NUMBER_OF_AREA_PER_WIDTH 6  // Nombre de parcelle par cote
#define SOLAR_SYSTEM_SIZE 16000  // Taille d'un cote (c'est un carre)
#define SIZE_HEXAGON 1000  // Taille dans le referentiel de la map d'un seul cote d'un hexagone
#define NUMBER_OF_HEXAGON_PER_WIDTH (MAP_SIZE / (SIZE_HEXAGON * 1.5) + 1)
#define NUMBER_OF_HEXAGON_PER_HEIGHT (int)(MAP_SIZE / (SIZE_HEXAGON * SQRT3) + 2)

#define INIT_PLANET_COUNT 70
#define INIT_SHIP_COUNT 3
#define ORE_TYPE_COUNT 5
#define CST_TEXT_NUMBER 85
#define FONT_NUMBER 1
#define SONGS_NUMBER 11
#define BUILD_TYPE_COUNT 12
#define ASTEROID_COUNT 500 // nombre d'astéroids par système solaire
#define MAX_COMPARTMENTS_PER_SHIP 4

#define SPRITE_SHEETS_DELAY 100  // En nombre de tick
#define WAIT_TIME_SHIP 1000  // En millisecondes
#define WAIT_TIME_PLANET 8000  // En millisecondes
#define REFRESH_TIME_BUILDS 1000  // En millisecondes
#define TANKS_UPDATE_INTERVAL 200  // En millisecondes
#define ASTEROID_TICK 300
#define ENEMY_GENERATION_PERIOD 1000000
#define REFRESH_TIME_TILE 500
#define REFRESH_TIME_WINDOW 500

#define STEP_TRANSLATION 40
#define LIMIT_UNZOOM 0.05
#define SHIP_SPEED 10.f
#define FPS 60
#define NAME_BACKUP "backups/backup1"

extern const uint32_t currentSeed;

#endif
