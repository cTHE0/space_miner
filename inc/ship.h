#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "ore.h"

typedef enum { MOVING_TO_TARGET, MOVING_TO_BASE, WAITING_ON_TARGET, WAITING_ON_BASE, OUT_OF_FUEL, BROKE } ShipState;

typedef enum { TRANSPORTER, EXPLORER, DEFENDER } ShipType;

typedef enum {SPOT_NONE, SPOT_PLANET, SPOT_POINT, SPOT_SHIP} SpotType;  // Vers quel genre se destination se dirige-t-on ?

typedef struct Ship Ship;  // Declaration anticipee (car sinon cercle de dépendance)

typedef struct {  // Notre destination
    SpotType type;
    union {
        Planet *planet;    // Si c'est une planète
        Ship *ship;        // Si c'est une fusee, par ex une station spatiale
        SDL_Point point;   // Si c'est un point libre sur la map, par ex pour partir en exploration
    };
} Spot;

struct Ship {
    ShipType shiptype;
    int id;
    int idModel;
    float x, y, w, h;      // Rect de la fusee sur la map
    SDL_Rect destRect;     // Rect de la fusee sur l'ecran physique
    Spot base;
    Spot target;
    float speed;
    ShipState state;
    int maxLife, currentLife;
    Cargo cargo;
    Uint32 lastRefreshFilling;
    int fuelConsumption;
    int range;
    Uint32 waitStartTime;
    int frameIndex;
    Uint32 lastFrameTime;
};


void initShips(Ship **ships, int shipCount, Planet *planets, int planetCount);
void updateShips(Ship *ships, int shipCount);
    void updateShipAnimation(Ship *ship, Uint32 currentTime);
    void updateShipMove(Ship *ship, Uint32 currentTime);
    void updateShipTanks(Ship *ship, Uint32 currentTime);
    
void renderShips(SDL_Texture ***textureShip, Ship *ships, int shipCount);
    void renderShipImage(SDL_Texture *spriteSheet, Ship ship, SDL_Point ShipOnScreen);
    void renderShipBars(Ship ship, SDL_Point ShipOnScreen);

void destroyShips(Ship *ships, int shipCount);

#endif
