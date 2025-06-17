#ifndef ENEMY_H
#define ENEMY_H

#include "ship.h"

void generateEnemies(Ship **ships, int *shipCount);
void generateEnemy(Ship targetShip, Ship **ships, int *shipCount);

#endif
