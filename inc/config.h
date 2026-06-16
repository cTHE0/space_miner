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
#define INIT_SHIP_COUNT 1
#define ORE_TYPE_COUNT 5
#define CST_TEXT_NUMBER 121
#define VICTORY_SCORE 25000  // Score a atteindre pour la victoire
#define DEFENDER_COST 350    // Cout en credits d'un vaisseau defenseur
#define FONT_NUMBER 1
#define SONGS_NUMBER 13
#define BUILD_TYPE_COUNT 12
#define ASTEROID_COUNT 500 // nombre d'astéroids par système solaire
#define MAX_COMPARTMENTS_PER_SHIP 4
#define DAMAGE 1 // Dégâts subis à chaque fois que ship touché par laser
#define STARTING_ORE_STOCK 350  // Stock initial de fuel et de fer sur les planètes de départ

#define SPRITE_SHEETS_DELAY 100  // En nombre de tick
#define WAIT_TIME_SHIP 1000  // En millisecondes
#define WAIT_TIME_PLANET 8000  // En millisecondes
#define REFRESH_TIME_BUILDS 1000  // En millisecondes
#define TANKS_UPDATE_INTERVAL 200  // En millisecondes
#define ASTEROID_TICK 300
#define ENEMY_GENERATION_PERIOD 11000 // Un nouveau pirate toutes les 11 s au maximum (modulé par la menace)
#define MAX_ENEMIES 26                // Plafond absolu de pirates simultanes
#define ENEMY_LIFE 12                 // Points de vie d'un pirate
#define TOWER_FIRE_PERIOD 550         // Cadence de tir des tours de defense (ms)
#define RAID_RANGE 1400               // Distance a laquelle un pillard commence a piller
#define RAID_PERIOD 2500              // Cadence de pillage (ms)
#define RAID_STEAL 80                 // Minerai vole par tick de pillage et par reservoir
#define LASER_GENERATION_PERIOD 800
#define REFRESH_TIME_TILE 500
#define REFRESH_TIME_WINDOW 500
#define REFRESH_TIME_KILLED_SHIP 500

#define STEP_TRANSLATION 40
#define LIMIT_UNZOOM 0.05
#define MIN_CAMERA_SCALE 0.32f  // Dézoom maximal : on reste proche des planètes
#define MAX_CAMERA_SCALE 2.0f   // Zoom maximal
#define SHIP_SPEED 10.f
#define FPS 60
#define NAME_BACKUP "backups/backup1"

extern const uint32_t currentSeed;

#endif
