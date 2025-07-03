#ifndef ENEMY_H
#define ENEMY_H

#include "ship.h"

typedef struct {
    SDL_Rect rect;
    SDL_Point target_coords;
    double angle;
    int speed;
    int i; //indice du laser
} Laser;

void addLaser(SDL_Rect rect, SDL_Point target_coords, float angle, int speed);
void deleteLaser(int i);
void updateEnemies(Ship **ships, int *shipCount);
void updateLasers(Ship **ships, int *shipCount, Mix_Chunk **sounds);
void renderLasers(SDL_Texture ***imageTextures);

void generateEnemy(Ship targetShip, Ship **ships, int *shipCount);
#endif
