#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "planet.h"
#include "ship.h"

#define carre(x) (x) * (x)
#define SQRT3 1.73205080757d

void *supprElemList(void *list, int *nb_elem, int type_size, int i);
void *swapElements(void *list, int nb_elem, int type_size, int i, int j);
void *moveToEnd(void *list, int nb_elem, int type_size, int index);

float distanceShipPlanet(Ship *ship, Planet *planet);
float distancePlanetPlanet(Planet *planet1, Planet *planet2);
float distanceShipShip(Ship *ship1, Ship *ship2);
float distancePointPoint(SDL_Point *p1, SDL_Point *p2);

void initArrayOfSyllabes(void);
void generateRandomName(char randomString[64], unsigned int seed, int index);

uint32_t generateRandNb32(uint32_t seed, int index);  // Pas efficace pour les petits entiers
uint8_t generateRandNb8(uint32_t seed, int index);  // Alternative pour les petits entiers

void plotPath(SDL_Point origin, SDL_Point destination, int dashLength, int gapLength, SDL_Color color);

void SDL_DrawEdgeOfRect(SDL_Rect rect, int lineWidth, SDL_Color color);
void SDL_DrawEdgeOfRect2(SDL_Rect rect, int lineWidth);

int power(int number, int power);
int extractOneDigit(int number, int position);
int countDigits(int n);

double computeAngleDeg(int x, int y, int cx, int cy);

SDL_Texture* createTextTextureWithNewline(TTF_Font* font, const char* text, SDL_Color color);

void drawCircle(SDL_Color color, int xc, int yc, int r);
void drawHexagon(SDL_Point center, int sizeSide);


#endif
