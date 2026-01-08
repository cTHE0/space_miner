#ifndef TILE_H
#define TILE_H

#include "config.h"
#include "tools.h"


void initTiles(Planet *planets);
void displayTiles(SDL_Texture ***imageTextures);

void updateTiles(Ship *ships, int shipCount);

void setBit(int i, int j, int value);
int getBit(int i, int j);

int *getByteCount(void);
void setByteCount(void);
uint8_t **getTilesMatrix(void);

void destroyTiles(void);


#endif
