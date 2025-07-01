#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ship.h"
#include "ore.h"

typedef enum { MOVING_TO_TARGET, MOVING_TO_BASE, WAITING_ON_TARGET, WAITING_ON_BASE, OUT_OF_FUEL, BROKE, STOPPED_MOVING_TO_TARGET, STOPPED_MOVING_TO_BASE, STOPPED_WAITING_ON_TARGET, STOPPED_WAITING_ON_BASE } ShipState;

typedef enum { TRANSPORTER, EXPLORER, DEFENDER, ENEMY } ShipType;

typedef enum {SPOT_NONE, SPOT_PLANET, SPOT_POINT, SPOT_SHIP} SpotType;  // Vers quel genre se destination se dirige-t-on ?

typedef struct Ship Ship;  // Declarations anticipees (car sinon cercle de dependance)
typedef struct Planet Planet;

typedef struct {  // Notre destination
    SpotType type;
    union {
        Planet *planet;    // Si c'est une planete
        Ship *ship;        // Si c'est une fusee, par ex une station spatiale
        SDL_Point point;   // Si c'est un point libre sur la map, par ex pour partir en exploration
    };
} Spot;

struct Ship {
    ShipType shiptype;
    int id;
    int idModel;
    int level;
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
    double angleWithPlanet; // Angle permettant de situer le ship sur la surface de la planète
    int noise;              // bruit, ondes EM émisent et attirant ennemis entre 0 et 100
};

void addShip(Ship newShip, Ship **ships, int *shipCount);
void initShips(Ship **ships, int shipCount, Planet *planets);
void updateShips(Ship *ships, int shipCount);
    void updateShipAnimation(Ship *ship, Uint32 currentTime);
    void updateShipMove(Ship *ship, Uint32 currentTime);
    void updateShipTanks(Ship *ship, Uint32 currentTime);
        void fuelConsumption(Ship *ship);
        void OreFillingOrEmptying(Ship *ships);
        void isShipOnPlanet(Ship *ship);
    
int haveFuel(Ship *ship);

void displayShips(SDL_Texture ***textureShip, Ship *ships, int shipCount);
    void renderShipImage(SDL_Texture *spriteSheet, Ship ship, SDL_Point ShipOnScreen);
    void renderShipBars(Ship ship, SDL_Point ShipOnScreen);

int globalTankCapacity(Ship *ship);

void destroyShips(Ship *ships, int shipCount);

#endif
