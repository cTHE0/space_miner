#include "ship.h"
#include "camera.h"
#include "config.h"
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SHIP_SPEED 0.1f
#define WAIT_TIME 1000  // 1000 millisecondes

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
        (*ships)[i].frameIndex = 0;
        (*ships)[i].lastFrameTime = 0;
    }
}

void updateShips(Ship *ships, int count, Planet *planets, int planetCount) {
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < count; i++) {
        if (SDL_GetTicks() > ships[i].lastFrameTime + SHIP_FRAME_DELAY) {
            ships[i].frameIndex = (ships[i].frameIndex + 1) % 4; // 4 images dans le sprite sheet
            ships[i].lastFrameTime = SDL_GetTicks();
        }

        if (ships[i].state == MOVING_TO_TARGET || ships[i].state == RETURNING) {
            Planet *destination = (ships[i].state == MOVING_TO_TARGET) ? ships[i].target : ships[i].base;
            float dx = destination->x - ships[i].x;
            float dy = destination->y - ships[i].y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance - ships[i].speed > destination->radius) {
                ships[i].x += (dx / distance) * ships[i].speed;
                ships[i].y += (dy / distance) * ships[i].speed;
            } else {
                if (ships[i].state == MOVING_TO_TARGET) {
                    ships[i].state = WAITING;
                    ships[i].waitStartTime = currentTime;
                } else if (ships[i].state == RETURNING) {
                    ships[i].state = WAITING;
                    ships[i].waitStartTime = currentTime;
                } else {
                    ships[i].target = &planets[rand() % planetCount];
                    ships[i].state = MOVING_TO_TARGET;
                }
            }
        } else if (ships[i].state == WAITING) {
            if (currentTime - ships[i].waitStartTime > WAIT_TIME) {
                if (carre(ships[i].target->x - ships[i].x) + carre(ships[i].target->x - ships[i].x) >
                    carre(ships[i].base->x - ships[i].x) + carre(ships[i].base->x - ships[i].x))  {
                    ships[i].state = MOVING_TO_TARGET;
                } else {
                    ships[i].state = RETURNING;
                }
            }
        }
    }
}

void renderShips(SDL_Renderer *renderer, SDL_Texture *spriteSheet, Ship *ships, int count) {
    float angle;

    for (int i = 0; i < count; i++) {
        // Calcul de la rotation du triangle selon la direction du déplacement
        if (ships[i].state == MOVING_TO_TARGET) {
            angle = atan2(ships[i].target->y - ships[i].y, ships[i].target->x - ships[i].x);
        } else {
            angle = atan2(ships[i].y - ships[i].target->y, ships[i].x - ships[i].target->x);
        }

        /*
        float size = 8; // Taille FIXE du vaisseau (ne dépend plus de camera.scale)

        // Calcul des coordonnées à l'écran (avec zoom pour la position, mais pas pour la taille)
        float screenX = (ships[i].x - camera.x) * camera.scale + SCREEN_WIDTH / 2;
        float screenY = (ships[i].y - camera.y) * camera.scale + SCREEN_HEIGHT / 2;

        // Point origine rect
        ships[i].x = screenX;
        ships[i].y = screenY; */

        // Dessin du vaisseau
        SDL_Rect srcRect = {ships[i].frameIndex * 64, 0, 64, 64}; // Frame actuelle sur le sprite sheet
        SDL_Rect destRect = {ships[i].x, ships[i].y, 64, 64};       // Position et taille affichée

        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);

    }
}


