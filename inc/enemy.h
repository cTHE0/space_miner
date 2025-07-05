#ifndef ENEMY_H
#define ENEMY_H


#include <SDL2/SDL.h>
#include "ship.h"


typedef struct {
    SDL_Rect rect;
    SDL_Point target_coords;
    double angle;
    int speed;
    int target_id; //indice du ship qui est visé
} Laser;

void addLaser(SDL_Rect rect, SDL_Point target_coords, int target_id, float angle, int speed);
void deleteLaser(int i);
int newTarget(Ship *ships, int shipCount, int ennemy_index, int idException);
void displayLasers(SDL_Texture ***imageTextures);

void generateEnemy(Ship targetShip, Ship **ships, int *shipCount);

void updateWarSystem(Ship **ships, int *shipCount, Mix_Chunk **sounds);
    void updateEnemies(Ship **ships, int *shipCount);
    void updateLasers(Ship *ships, Mix_Chunk **sounds);
    void newLasersFired(Ship *ships, int shipCount, Mix_Chunk **sounds);

int getLasersCount(void);
Laser *getLasersList(void);


#endif
