#include "solar_system.h"

#include "planet.h"
#include "config.h"
#include "tools.h"
#include "config.h"


void solarSystemCoordinator(int *nbEntityGenerated, Planet *planets, int planetCount) {
    // Initialisation du nombre d'entites par systeme solaire
    static int maxEntity = 20;
    static int minEntity = 5;
    int nbEntityNewSS = minEntity + generateRandNb8(currentSeed, *nbEntityGenerated) % (maxEntity - minEntity + 1);  // Nb d'entites à rajouter ds le nouveau système solaire

    if (*nbEntityGenerated + nbEntityNewSS > planetCount){  // Le nouveau systeme solaire passe-t-il ?
        nbEntityNewSS = planetCount - *nbEntityGenerated;
    }

    // Generation du nouveau systeme solaire
    generateSolarSystem(planets, generateRandNb32(currentSeed, *nbEntityGenerated) % MAP_SIZE, generateRandNb32(currentSeed, *nbEntityGenerated + 1) % MAP_SIZE, *nbEntityGenerated, nbEntityNewSS);

    // Actualisation du nombre d'entites deja generes
    *nbEntityGenerated += nbEntityNewSS;
}

void generateSolarSystem(Planet *planets, int x, int y, int nbEntityGenerated, int nbEntityNewSS) { 
    static int randIndex = 0;

    // Initialisation du soleil
    Planet *newSun = &planets[nbEntityGenerated];

    newSun->planetType = SUN;
    newSun->id = nbEntityGenerated;
    newSun->regenerationTime = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;  // En millisecondes
    randIndex ++;
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        newSun->maxOre[i] = 1 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;
        randIndex ++;
        newSun->currentOre[i] = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (int)newSun->maxOre[i];  // Les planetes ne sont pas toutes initialement remplies
        randIndex ++;
    }
    int r = 150 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 50;
    randIndex ++;
    newSun->radius = r;
    newSun->x = x;
    newSun->y = y;
    newSun->buildingCounter = 1;
    newSun->buildings = malloc(newSun->buildingCounter * sizeof(Building));

    // Generation des planetes
    for (int j = nbEntityGenerated + 1; j < nbEntityGenerated + nbEntityNewSS; j++) {
        planets[j].id = j;
        planets[j].planetType = PLANET;
        planets[j].buildings = malloc(planets[j].buildingCounter * sizeof(Building));
        planets[j].regenerationTime = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;  // En millisecondes
        randIndex ++;
        
        for (int i = 0; i < ORE_TYPE_COUNT; i++) {
            planets[j].maxOre[i] = 1 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;
            randIndex ++;
            planets[j].currentOre[i] = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (int)planets[j].maxOre[i];  // Les planetes ne sont pas toutes initialement remplies
            randIndex ++;
        }

        // Recherche d'une nouvelle planete eloignee de toutes les autres
        short planetIsAlone = 0;  // 0: la nouvelle planete est proche d'une autre, 1: la nouvelle planete est eloignee
        int rp, xp, yp;
        while (planetIsAlone == 0) {
            rp = 25 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 30;
            randIndex ++;
            xp = x + (2000 - generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 4000); // Abscisse planète
            randIndex ++;
            yp = y + (2000 - generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 4000);
            randIndex ++;
            planetIsAlone = 1;  // La nouvelle planete est eloignee A PART SI l'on en detecte une autre a cote

            for (int k = nbEntityGenerated; k < j; k++) {  // Verifie qu'il n'y ait pas de planetes trop proches de la planete nbEntityGenerated
                if (planetIsAlone == 1 && 
                    (abs(xp - (int)planets[k].x) < rp + planets[k].radius &&
                     abs(yp - (int)planets[k].y) < rp + planets[k].radius)) {
                    planetIsAlone = 0;
                    break;
                }
            }
        }
        planets[j].radius = rp;
        planets[j].x = xp;
        planets[j].y = yp;
    }
}
