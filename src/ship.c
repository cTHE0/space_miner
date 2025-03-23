#include "ship.h"
#include "camera.h"
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SHIP_SPEED 0.5f
#define WAIT_TIME 10000  // 10 secondes en millisecondes

void initShips(Ship *ships, int count, Planet *planets, int planetCount) {
    for (int i = 0; i < count; i++) {
        ships[i].base = &planets[0]; // La première planète est la base
        ships[i].target = &planets[rand() % planetCount];
        ships[i].x = ships[i].base->x;
        ships[i].y = ships[i].base->y;
        ships[i].speed = SHIP_SPEED;
        ships[i].state = MOVING_TO_TARGET;
        ships[i].waitStartTime = 0;
    }
}

void updateShips(Ship *ships, int count, Planet *planets, int planetCount) {
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < count; i++) {
        Ship *ship = &ships[i];

        if (ship->state == MOVING_TO_TARGET || ship->state == RETURNING) {
            Planet *destination = (ship->state == MOVING_TO_TARGET) ? ship->target : ship->base;
            float dx = destination->x - ship->x;
            float dy = destination->y - ship->y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > ship->speed) {
                ship->x += (dx / distance) * ship->speed;
                ship->y += (dy / distance) * ship->speed;
            } else {
                ship->x = destination->x;
                ship->y = destination->y;

                if (ship->state == MOVING_TO_TARGET) {
                    ship->state = WAITING;
                    ship->waitStartTime = currentTime;
                } else {
                    ship->target = &planets[rand() % planetCount];  // ✅ Correction ici
                    ship->state = MOVING_TO_TARGET;
                }
            }
        } else if (ship->state == WAITING) {
            if (currentTime - ship->waitStartTime > WAIT_TIME) {
                ship->state = RETURNING;
            }
        }
    }
}

void renderShips(SDL_Renderer *renderer, Ship *ships, int count) {
    for (int i = 0; i < count; i++) {
        Ship *ship = &ships[i];

        // Calcul de la rotation du triangle selon la direction du déplacement
        float dx = ship->target->x - ship->x;
        float dy = ship->target->y - ship->y;
        float angle = atan2(dy, dx);

        float size = 8 * camera.scale; // Taille du vaisseau

        // Points du triangle
        float x1 = ship->x + cos(angle) * size;
        float y1 = ship->y + sin(angle) * size;
        float x2 = ship->x + cos(angle + 2.5f) * size;
        float y2 = ship->y + sin(angle + 2.5f) * size;
        float x3 = ship->x + cos(angle - 2.5f) * size;
        float y3 = ship->y + sin(angle - 2.5f) * size;

        filledTrigonRGBA(renderer,
            (int)((x1 - camera.x) * camera.scale), (int)((y1 - camera.y) * camera.scale),
            (int)((x2 - camera.x) * camera.scale), (int)((y2 - camera.y) * camera.scale),
            (int)((x3 - camera.x) * camera.scale), (int)((y3 - camera.y) * camera.scale),
            255, 255, 0, 255);
    }
}
