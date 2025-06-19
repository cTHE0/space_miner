#ifndef TILE_H
#define TILE_H

#include "config.h"
#include "tools.h"


void initTiles(void);
void displayTiles(void);

void updateTiles(Ship *ships, int shipCount);

void setBit(uint8_t *array, int index, int value);
int getBit(uint8_t *array, int index);

void destroyTiles(void);


#endif
