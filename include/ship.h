#ifndef SHIP_H
#define SHIP_H

#include "planet.h"
#include "ore.h"
#include <SDL2/SDL.h>

typedef enum { MOVING_TO_TARGET, WAITING, RETURNING } ShipState;

typedef struct {
    float x, y;             // Position du vaisseau
    Planet *base;           // Planète de départ
    Planet *target;         // Planète cible
    float speed;            // Vitesse du vaisseau
    ShipState state;        // État du vaisseau
    int life;
    int comportment_capacity;
    Compartment *compartments;
    Uint32 waitStartTime;   // Temps d’attente à destination
} Ship;

void initShips(Ship **ships, int count, Planet *planets, int planetCount);
void updateShips(Ship *ships, int count, Planet *planets, int planetCount);
void renderShips(SDL_Renderer *renderer, Ship *ships, int count);

#endif
