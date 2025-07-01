#include <SDL2/SDL.h>
#include <stdlib.h>
#include "asteroid.h"
#include "config.h"
#include "planet.h"
#include "renderer.h"
#include "assets_gestion.h"
#include "camera.h"
#include <math.h>


static SDL_Rect (*asteroids)[ASTEROID_COUNT];  // asteroids[0] = liste des ASTEROID_COUNT asteroid associés 1er soleil généré
static double asteroidsData[ASTEROID_COUNT][3];


void initAngleSpeed(void) {
    for (int i = 0; i < ASTEROID_COUNT; i++) {
        asteroidsData[i][0] = i * 360.f / ASTEROID_COUNT;  // Position angulaire
        asteroidsData[i][1] = (10 + rand() % 30) * 0.001;  // Vitesse angulaire
        asteroidsData[i][2] = SOLAR_SYSTEM_SIZE * 0.75 * (1 + (rand() % 10) / 100.f); // Distance du soleil
    }
}

void initAsteroids(Planet *planets, int planetCount) {
    initAngleSpeed();

    asteroids = malloc(getNbSolarSystems() * sizeof(SDL_Rect[ASTEROID_COUNT]));

    int counterSun = 0;
    for (int i = 0; i < planetCount; i++) {
        if (planets[i].planetType == SUN) { 
            for (int j = 0; j < ASTEROID_COUNT; j++) {
                asteroids[counterSun][j] = (SDL_Rect){0, 0, planets[i].radius / (3 + j % 5), planets[i].radius / (4 + j % 3)};
            }
            counterSun++;
        }
    }
}

void updateAsteroids(Planet *planets, int planetCount) {
    double angle;  // en radian
    int counterSun = 0;

    for (int j = 0; j < ASTEROID_COUNT; j++) {
        asteroidsData[j][0] += asteroidsData[j][1];
        if (asteroidsData[j][0] > 360) {
            asteroidsData[j][0] -= 360;
        }
    }

    for (int i = 0; i < planetCount; i++) {
        if (planets[i].planetType == SUN) {
            for (int j = 0; j < ASTEROID_COUNT; j++) {
                angle = (i * 10 + asteroidsData[j][0]) * M_PI / 180;
                asteroids[counterSun][j].x = planets[i].x + asteroidsData[j][2] * cos(angle);
                asteroids[counterSun][j].y = planets[i].y + asteroidsData[j][2] * sin(angle);
            }
            counterSun++;
        }
    }
}

void displayAsteroid(SDL_Texture ***imageTextures) {
    float screenX;
    float screenY;
    float screenRadius;

    for (int i = 0; i < getNbSolarSystems(); i++) {
        for (int j = 0; j < ASTEROID_COUNT; j++) {
            screenX = (asteroids[i][j].x - getCameraRect().x - asteroids[i][j].w - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
            screenY = (asteroids[i][j].y - getCameraRect().y - asteroids[i][j].w - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;  
            screenRadius = asteroids[i][j].w * getCameraScale();
            SDL_RenderCopyEx(renderer, imageTextures[0][i % 2], NULL, &(SDL_Rect){screenX, screenY, screenRadius, screenRadius}, 14 * asteroidsData[j][0] * (1 - 2 * i % 2), NULL, SDL_FLIP_NONE);
        }
    }
}

void destroyAsteroids(void) {
    free(asteroids);
}
