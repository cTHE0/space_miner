#include "ship.h"
#include "camera.h"
#include "config.h"
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SHIP_SPEED 0.1f
#define WAIT_TIME 1000  // 10 secondes en millisecondes

void initShips(Ship **ships, int count, Planet *planets, int planetCount) {
    *ships = malloc(count * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation mémoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *ships après un échec
    }

    for (int i = 0; i < count; i++) {
        (*ships)[i].base = &planets[0]; // La première planète est la base
        (*ships)[i].target = &planets[rand() % planetCount];
        (*ships)[i].x = (*ships)[i].base->x;
        (*ships)[i].y = (*ships)[i].base->y;
        (*ships)[i].speed = SHIP_SPEED;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].waitStartTime = 0;
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
                    ship->target = &planets[rand() % planetCount];
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

        float size = 8; // Taille FIXE du vaisseau (ne dépend plus de camera.scale)

        // Calcul des coordonnées à l'écran (avec zoom pour la position, mais pas pour la taille)
        float screenX = (ship->x - camera.x) * camera.scale + SCREEN_WIDTH / 2;
        float screenY = (ship->y - camera.y) * camera.scale + SCREEN_HEIGHT / 2;


        // Points du triangle
        float x1 = screenX + cos(angle) * size;
        float y1 = screenY + sin(angle) * size;
        float x2 = screenX + cos(angle + 2.5f) * size;
        float y2 = screenY + sin(angle + 2.5f) * size;
        float x3 = screenX + cos(angle - 2.5f) * size;
        float y3 = screenY + sin(angle - 2.5f) * size;

        // Dessin du vaisseau
        filledTrigonRGBA(renderer, (int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3, 255, 255, 0, 255);
    }
}


