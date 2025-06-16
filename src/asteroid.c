#include <SDL2/SDL.h>
#include "asteroid.h"
#include "config.h"
#include "planet.h"
#include "renderer.h"
#include "assets_gestion.h"
#include "camera.h"
#include <math.h>

static SDL_Rect (*asteroids)[ASTEROID_COUNT]; //asteroids[0] = liste des ASTEROID_COUNT asteroid associés 1er soleil généré
static int angle_speed_asteroids[ASTEROID_COUNT][2];

void initAngleSpeed(){
    for (int i=0; i<ASTEROID_COUNT; i++) {
        angle_speed_asteroids[i][0] = (rand()%20) +  360/ASTEROID_COUNT; //angle
        angle_speed_asteroids[i][1] = 1 + rand()%3;
    }
}

void initAsteroids(Planet *planets, int planet_count) {
    initAngleSpeed();

    int nb_solar_systems = getNbSolarSystems();
    printf("%d, %d \n", INIT_PLANET_COUNT, planet_count);
    asteroids = malloc(nb_solar_systems*sizeof(SDL_Rect[ASTEROID_COUNT]));
    int s = 0;
    printf("Nb solar syst: %d \n", nb_solar_systems);
    printf("testy: %d \n", planets[1].id);
    for (int i=0; i<planet_count; i++) {
        if (planets[i].planetType == SUN) {  
            if (s >= nb_solar_systems) {
                fprintf(stderr, "ERREUR: Trop de soleils par rapport à la mémoire allouée!\n");
                exit(EXIT_FAILURE);
            }
            printf("SUN! \n");
            for (int j=0; j<ASTEROID_COUNT; j++) {
                printf("GOOD[j=%d]\n", j);
                asteroids[s][j] = (SDL_Rect){planets[i].x+planets[i].radius/2 + SOLAR_SYSTEM_SIZE, planets[i].y+planets[i].radius/2, planets[i].radius, planets[i].radius};
            }
            printf("Fin génération syst %d \n", s);
            s++;
        }
    }
}

Uint32 last_asteroid_tick = 0;

void updateAsteroids(Planet *planets, int planetCount) {
    if (SDL_GetTicks() - last_asteroid_tick < ASTEROID_TICK) {
        return;
    }
    last_asteroid_tick = SDL_GetTicks();
    double rad_angle;
    int s=0;

    for (int j=0; j<ASTEROID_COUNT; j++) {
        angle_speed_asteroids[j][0] = (angle_speed_asteroids[j][0] + angle_speed_asteroids[j][1]) %360;
    }

    for (int i=0; i<planetCount; i++) {
        if (planets[i].planetType == SUN) {
            for (int j=0; j<ASTEROID_COUNT; j++) {
                rad_angle = angle_speed_asteroids[j][0] * M_PI / 180;
                asteroids[i][j].x = planets[i].x + planets[i].radius/2 + SOLAR_SYSTEM_SIZE*cos(rad_angle);
                asteroids[i][j].y = planets[i].y + planets[i].radius/2 + SOLAR_SYSTEM_SIZE*sin(rad_angle);
            }
        }
    }
}

void displayAsteroid(SDL_Texture ***imageTextures) {
    float screenX;
    float screenY;
    float screenRadius;
    for (int i=0; i<getNbSolarSystems(); i++) {
        for (int j=0; j<ASTEROID_COUNT; j++) {
            screenX = (asteroids[i][j].x - getCameraRect().x - asteroids[i][j].w - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;  // (planets[i].x, planets[i].y) = coordonnees sur la map
            screenY = (asteroids[i][j].y - getCameraRect().y - asteroids[i][j].w - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;  
            screenRadius = asteroids[i][j].w * getCameraScale();
            SDL_RenderCopy(renderer, imageTextures[0][i%2], NULL, &(SDL_Rect){screenX, screenY, screenRadius, screenRadius});
        }
    }
}

void freeAsteroid() {
    free(asteroids);
}