#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "ship.h"
#include "camera.h"
#include "config.h"


void initShips(Ship **ships, int count, Planet *planets, int planetCount) {
    *ships = malloc(count * sizeof(Ship));

    if (*ships == NULL) {
        printf("Erreur d'allocation mémoire pour les vaisseaux !\n");
        return;  // Sortir de la fonction pour éviter d'utiliser *ships après un échec
    }

    for (int i = 0; i < count; i++) {
        (*ships)[i].base = &planets[0]; // La première planète est la base de chaque vaisseau
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
        // Pour animation de la flamme des fusees 
        if (SDL_GetTicks() > ships[i].lastFrameTime + SHIP_FRAME_DELAY) {
            ships[i].frameIndex = (ships[i].frameIndex + 1) % 4; // 4 images dans le sprite sheet
            ships[i].lastFrameTime = SDL_GetTicks();
        }

        // Pour le deplacement des fusees dans l'espace
        if (ships[i].state == MOVING_TO_TARGET || ships[i].state == RETURNING) {
            Planet *destination = (ships[i].state == MOVING_TO_TARGET) ? ships[i].target : ships[i].base;
            float dx = destination->x - (ships[i].x + 64 / 2.f);
            float dy = destination->y - (ships[i].y + 64 / 2.f);
            float distance = sqrt(dx * dx + dy * dy);

            if (distance - ships[i].speed >= destination->radius) {
                ships[i].x += dx * ships[i].speed / distance;
                ships[i].y += dy * ships[i].speed / distance;
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
            if (currentTime - ships[i].waitStartTime > WAIT_TIME_SHIP) {
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
    float target_x, target_y;
    float angle;
    float screenX, screenY;
    SDL_Rect srcRect, destRect;
    SDL_Point center;
        
    for (int i = 0; i < count; i++) {
        // Calcul de l'angle en degrés
        target_x = ships[i].target->x;
        target_y = ships[i].target->y;
        angle = atan2(target_y - (ships[i].y + 64 / 2.f), target_x - (ships[i].x + 64 / 2.f)) * 180.0f / M_PI;
        angle += (ships[i].state == MOVING_TO_TARGET) ? 90.0f : -90.0f;  // Si la fusée ne va pas vers la cible, on l'inverse

        // Calcul des coordonnées à l'écran
        screenX = (ships[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;
        screenY = (ships[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;

        srcRect.x = ships[i].frameIndex * 64; // Frame actuelle sur le sprite sheet
        srcRect.y = 0;
        srcRect.h = 64;
        srcRect.w = 64;

        destRect.x = screenX;   // Position et taille affichée
        destRect.y = screenY;
        destRect.h = 64 * camera.scale;
        destRect.w = 64 * camera.scale;

        // Définition du point de rotation (au centre du sprite)
        center.x = destRect.w / 2;
        center.y = destRect.h / 2;

        // Dessin avec rotation
        SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
    }
}


