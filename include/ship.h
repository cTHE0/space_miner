#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>
#include "planet.h"
#include "ore.h"

typedef enum { MOVING_TO_TARGET, WAITING, RETURNING } ShipState;

typedef enum {BASIC, EXPLORER, DEFENDER} ShipType;

typedef struct {
    ShipType shiptype;          // Type de vaisseau (utile pr définir ensuite son comportement, stats, render)
    float x, y;                 // Position du vaisseau
    Planet *base;               // Planète de départ
    Planet *target;             // Planète cible
    float speed;                // Vitesse du vaisseau
    ShipState state;            // Etat du vaisseau
    float maxLife, currentLife; // Vie des fusees
    Cargo cargo;                // Cargaison de la fusée
    Uint32 waitStartTime;       // Temps d’attente à destination  (a remplacer par le temps de minage/depot !!-> besoin d'ameliorer les stats des fusees)
    int frameIndex;             // Numero de l'image prise dans le spritesheet
    Uint32 lastFrameTime;       // Date au dernier changement de frame du spritesheet
} Ship;

void initShips(Ship **ships, int count, Planet *planets, int planetCount);
void updateShips(Ship *ships, int count, Planet *planets, int planetCount);
void renderShips(SDL_Renderer *renderer, SDL_Texture *spriteSheet, Ship *ships, int count);
void destroyShips(Ship *ships, int ship_count);
#endif
