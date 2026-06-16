#ifndef ENEMY_H
#define ENEMY_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "ship.h"
#include "planet.h"


typedef struct {
    SDL_Rect rect;
    SDL_Point target_coords;
    double angle;
    int speed;
    int target_id; //indice du ship qui est visé
    int damage;    //dégâts infligés à l'impact
} Laser;

void addLaser(SDL_Rect rect, SDL_Point target_coords, int target_id, float angle, int speed, int damage);
void deleteLaser(int i);
int newTarget(Ship *ships, int shipCount, int ennemy_index, int idException);
void displayLasers(SDL_Texture ***imageTextures);

void generateEnemy(int targetIndex, int raidPlanet, Ship **ships, int *shipCount, Planet *planets);

void updateWarSystem(Ship **ships, int *shipCount, Planet *planets, int planetCount, Mix_Chunk **sounds);
    void updateEnemies(Ship **ships, int *shipCount, Planet *planets, int planetCount);
    void updateLasers(Ship *ships, Mix_Chunk **sounds);
    void newLasersFired(Ship *ships, int shipCount, Mix_Chunk **sounds);
    void updateDefenceTowers(Planet *planets, int planetCount, Ship *ships, int shipCount, Mix_Chunk **sounds);
    void updateRaiders(Ship *ships, int shipCount, Planet *planets, int planetCount);
    void updateDefenders(Ship *ships, int shipCount, Mix_Chunk **sounds);

int getLasersCount(void);
Laser *getLasersList(void);


#endif
