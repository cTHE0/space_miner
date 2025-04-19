#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ore.h"

typedef enum { MOVING_TO_TARGET, MOVING_TO_BASE, WAITING_ON_TARGET, WAITING_ON_BASE, OUT_OF_FUEL, BROKE } ShipState;

typedef enum { TRANSPORTER, EXPLORER, DEFENDER } ShipType;

typedef struct {
    ShipType shiptype;          // Type de vaisseau (utile pr définir ensuite son comportement, stats, render)
    int idPicture;              // Chaque fusee a un skin particulier
    float x, y, w, h;           // Rect de la fusee sur la map
    SDL_Rect destRect;          // Rect de la fusee sur l'ecran physique
    Planet *base;               // Planète de départ
    Planet *target;             // Planète ciblee
    float speed;                // Vitesse de la fusee
    ShipState state;            // Etat de la fusee
    int maxLife, currentLife;   // Vie de la fusee
    Cargo cargo;                // Cargaison de la fusee (qui contient les compartiments)
    Uint32 lastRefreshFilling;  // Dernier instant ou la fusee a actualisee sa consommation d'essence
    int fuelConsumption;        // Dernier instant ou la fusee a actualisee sa consommation d'essence

    Uint32 waitStartTime;       // Temps d’attente à destination  (a remplacer par le temps de minage/depot !!-> besoin d'ameliorer les stats des fusees)
    int frameIndex;             // Numero de l'image prise dans le spritesheet
    Uint32 lastFrameTime;       // Date au dernier changement de frame du spritesheet
} Ship;

void initShips(Ship **ships, int shipCount, Planet *planets, int planetCount);
void updateShips(Ship *ships, int shipCount);
    void updateShipAnimation(Ship *ship, Uint32 currentTime);
    void updateShipMove(Ship *ship, Uint32 currentTime);
    void updateShipFuel(Ship *ship, Uint32 currentTime);
void renderShips(SDL_Renderer *renderer, SDL_Texture ***textureShip, Ship *ships, int shipCount);
    void renderShipImage(SDL_Renderer *renderer, SDL_Texture *spriteSheet, Ship ship, SDL_Point ShipOnScreen);
    void renderShipBars(SDL_Renderer *renderer, Ship ship, SDL_Point ShipOnScreen);

void destroyShips(Ship *ships, int shipCount);

extern Ship *ships;

#endif
