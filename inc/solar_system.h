#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include "planet.h"

void solarSystemCoordinator(int *nbEntityGenerated, Planet *planets, int planetCount);
void generateSolarSystem(Planet *planets, int x, int y, int i, int nb_planets_ss);

#endif
