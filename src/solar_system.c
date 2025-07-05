#include "solar_system.h"

#include "planet.h"
#include "config.h"
#include "tools.h"
#include "config.h"


static int maxEntity = 3;  // Par systeme solaire
static int minEntity = 7;

static int randIndex = 0;  // Pour que chaque planete n'est pas toutes ses donnees en fonction d'un meme entier aleatoire


int generateSolarSystem(Planet *planets, int planetCount, int nbEntityGenerated) {
    // Initialisation du nombre d'entites par systeme solaire
    int nbEntityNewSS = minEntity + generateRandNb8(currentSeed, nbEntityGenerated) % (maxEntity - minEntity + 1);  // Nb d'entites a rajouter ds le nouveau systeme solaire

    if (nbEntityGenerated + nbEntityNewSS > planetCount){  // Le nouveau systeme solaire passe-t-il ?
        nbEntityNewSS = planetCount - nbEntityGenerated;
    }

    // Generation des entites du systeme solaire
    for (int j = nbEntityGenerated; j < nbEntityGenerated + nbEntityNewSS; j++) {
        memset(&planets[j], 0, sizeof(Planet));
        planets[j].id = j;

        // Gestion abondance
        for (int i = 0; i < 5; i++) {
            planets[j].abundance[i] = 5 + generateRandNb8(currentSeed, nbEntityGenerated + randIndex) % 96; randIndex ++;
        }
        
        // Gestion des reservoirs
        for (int i = 0; i < ORE_TYPE_COUNT; i++) {
            planets[j].builds[i].tank.maxCapacity = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000; randIndex ++;
            planets[j].builds[i].tank.currentCapacity = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (int)planets[j].builds[i].tank.maxCapacity; randIndex ++;  // Les planetes ne sont pas toutes initialement remplies randIndex ++;
        }

        // Si c'est un soleil (premier element genere dans un ss)
        if (j == nbEntityGenerated) {
            planets[j].planetType = SUN;
            freeAreaForSS(planets, nbEntityGenerated);
            planets[j].radius = 1000 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 200; randIndex ++;
        } 

        // Si ce n'est pas un soleil
        else {                       
            planets[j].planetType = PLANET;
            planets[j].radius = 400 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 30; randIndex ++;
            freeAreaForPlanet(planets, j, nbEntityGenerated);
        } 
    }

    // Pour actualisation le nombre d'entites deja generes
    return nbEntityNewSS;
}

void freeAreaForSS(Planet *planets, int nbEntityGenerated) {
    // Initialisation des coordonnees du centre du ss i.e. du soleil
    planets[nbEntityGenerated].x = MAP_SIZE / 2;
    planets[nbEntityGenerated].y = MAP_SIZE / 2;

    // Recherche d'un endroit libre de tout ss
    int freeArea = 0;
    while (freeArea == 0) {
        freeArea = 1;
        for (int i = 0; i < nbEntityGenerated; i++) {
            if (planets[i].planetType == SUN &&
                fabsf(planets[i].x - planets[nbEntityGenerated].x) < 1.7f * SOLAR_SYSTEM_SIZE &&  // 1.7f : prise en compte des asteroides
                fabsf(planets[i].y - planets[nbEntityGenerated].y) < 1.7f * SOLAR_SYSTEM_SIZE) {
                freeArea = 0;
                planets[nbEntityGenerated].x = SOLAR_SYSTEM_SIZE + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (MAP_SIZE - 2 * SOLAR_SYSTEM_SIZE); randIndex ++;
                planets[nbEntityGenerated].y = SOLAR_SYSTEM_SIZE + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (MAP_SIZE - 2 * SOLAR_SYSTEM_SIZE); randIndex ++;
                break;
            }
        }
    }
}

void freeAreaForPlanet(Planet *planets, int currentIndexPlanet, int nbEntityGenerated) {
    // Recherche d'une nouvelle planete eloignee de toutes les autres
    int freeArea = 0;
    while (freeArea == 0) {
        planets[currentIndexPlanet].x = planets[nbEntityGenerated].x - SOLAR_SYSTEM_SIZE / 2 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % SOLAR_SYSTEM_SIZE; randIndex ++;
        planets[currentIndexPlanet].y = planets[nbEntityGenerated].y - SOLAR_SYSTEM_SIZE / 2 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % SOLAR_SYSTEM_SIZE; randIndex ++;
        freeArea = 1;
        for (int i = nbEntityGenerated; i < currentIndexPlanet; i++) {
            if (fabsf(distancePlanetPlanet(&planets[nbEntityGenerated], &planets[i]) - distancePlanetPlanet(&planets[nbEntityGenerated], &planets[currentIndexPlanet])) < planets[currentIndexPlanet].radius + planets[i].radius) {
                freeArea = 0;
                break;
            }
        }
    }
}

