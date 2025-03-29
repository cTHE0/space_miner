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
        (*ships)[i].speed = (rand() / (float)RAND_MAX + 0.001) * SHIP_SPEED;
        (*ships)[i].state = MOVING_TO_TARGET;
        (*ships)[i].waitStartTime = 0;
        (*ships)[i].frameIndex = rand() % 4;  // Desynchronisation des fusees
        (*ships)[i].lastFrameTime = 0;
        (*ships)[i].maxLife = 100;
        (*ships)[i].currentLife = rand() % (int)(*ships)[i].maxLife;
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
    for (int i = 0; i < count; i++) {
        // Calcul de l'angle en degrés
        float target_x = ships[i].target->x;
        float target_y = ships[i].target->y;
        float angle = atan2(target_y - (ships[i].y + 64 / 2.f), target_x - (ships[i].x + 64 / 2.f)) * 180.0f / M_PI;
        angle += (ships[i].state == MOVING_TO_TARGET) ? 90.0f : -90.0f;  // Si la fusée ne va pas vers la cible, on l'inverse

        // Calcul des coordonnées à l'écran
        float screenX = (ships[i].x - camera.rect.x - SCREEN_WIDTH / 2.f) * camera.scale + SCREEN_WIDTH / 2.f;
        float screenY = (ships[i].y - camera.rect.y - SCREEN_HEIGHT / 2.f) * camera.scale + SCREEN_HEIGHT / 2.f;

        SDL_Rect srcRect = {ships[i].frameIndex * 64, 0, 64, 64}; // Frame actuelle sur le sprite sheet

        SDL_Rect destRect = {screenX, screenY, 64 * camera.scale, 64 * camera.scale};  // Position et taille affichée

        // Définition du point de rotation (au centre du sprite)
        SDL_Point center = {destRect.w / 2, destRect.h / 2};

        // Dessin des fusees avec rotation
        SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);

        // Dessin de la barre de vie
        destRect.h = 1 * camera.scale;
        destRect.y -= destRect.h + 2 / camera.scale;

        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_RenderFillRect(renderer, &destRect);

        destRect.w *= ships[i].currentLife / ships[i].maxLife;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}


