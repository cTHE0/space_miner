#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include "planet.h"


int generateSolarSystem(Planet *planets, int planetCount, int nbEntityGenerated);
void freeAreaForSS(Planet *planets, int nbEntityGenerated);
void freeAreaForPlanet(Planet *planets, int currentIndexPlanet, int nbEntityGenerated);


#endif
