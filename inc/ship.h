#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "ore.h"

typedef enum { MOVING_TO_TARGET, MOVING_TO_BASE, WAITING_ON_TARGET, WAITING_ON_BASE, OUT_OF_FUEL, BROKE } ShipState;

typedef enum { TRANSPORTER, EXPLORER, DEFENDER } ShipType;

typedef enum { // Vers quel genre se destination se dirige-t-on ?
    TARGET_NONE,
    TARGET_PLANET,
    TARGET_POINT,
    TARGET_STATION,
} TargetType;

// Déclaration anticipée (car sinon cercle de dépendance)
struct Ship;
typedef struct Ship Ship;

typedef struct { // Notre destination
    TargetType type;
    union {
        Planet *planet;    // Si c'est une planète
        Ship *ship;        // Si c'est un ship, par ex une station spatiale
        struct {
            int x, y;      // Si c'est un point libre sur la map, par ex pour partir en exploration
        } point;
    };
} Target;


struct Ship {
    ShipType shiptype;
    int id;
    int idModel;
    float x, y, w, h;
    SDL_Rect destRect;
    Planet *base;
    Target target;
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
    void updateShipFuel(Ship *ship, Uint32 currentTime);
    
void renderShips(SDL_Texture ***textureShip, Ship *ships, int shipCount);
    void renderShipImage(SDL_Texture *spriteSheet, Ship ship, SDL_Point ShipOnScreen);
    void renderShipBars(Ship ship, SDL_Point ShipOnScreen);

void destroyShips(Ship *ships, int shipCount);

SDL_Rect getShipOnScreen(Ship ship);

#endif
