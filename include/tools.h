#ifndef TOOLS_H
#define TOOLS_H

#include "ship.h"
#include "planet.h"

void *supprElemList(void *list, int *nb_elem, int type_size, int i);
void *swapElements(void *list, int nb_elem, int type_size, int i, int j);
void *moveToEnd(void *list, int nb_elem, int type_size, int index);
float distanceShipPlanet(Ship ship, Planet planet);
float distancePlanetPlanet(Planet planet1, Planet planet2);
float distanceShipShip(Ship ship1, Ship ship2);
void initArrayOfSyllabes();
void generateRandomName(char *randomString, int seed);
void generatePlanetName(char *randomString, int x, int y);

#endif
