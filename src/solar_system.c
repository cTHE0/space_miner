#include "planet.h"
#include "config.h"
#include "tools.h"
#include "solar_system.h"


void solarSystemCoordinator(int *nbEntityGenerated, int planetCount) {
    // Initialisation du nombre d'entites par systeme solaire
    static int maxEntity = 20;
    static int minEntity = 5;
    int nbEntityNewSS = minEntity + generateRandNb8(currentSeed, *nbEntityGenerated) % (maxEntity - minEntity + 1);  // Nb d'entites à rajouter ds le nouveau système solaire

    if (*nbEntityGenerated + nbEntityNewSS > planetCount){  // Le nouveau systeme solaire passe-t-il ?
        nbEntityNewSS = planetCount - *nbEntityGenerated;
    }

    // Generation du nouveau systeme solaire
    generateSolarSystem(generateRandNb32(currentSeed, *nbEntityGenerated) % MAP_SIZE, generateRandNb32(currentSeed, *nbEntityGenerated + 1) % MAP_SIZE, *nbEntityGenerated, nbEntityNewSS);

    // Actualisation du nombre d'entites deja generes
    *nbEntityGenerated += nbEntityNewSS;
}

void generateSolarSystem(int x, int y, int nbEntityGenerated, int nbEntityNewSS) { 
    static int randIndex = 0;

    // Initialisation du soleil
    planets[nbEntityGenerated].id = nbEntityGenerated;
    planets[nbEntityGenerated].planetType = SUN;
    planets[nbEntityGenerated].maxOre = 1 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 1000;
    randIndex ++;
    planets[nbEntityGenerated].currentOre = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (int)planets[nbEntityGenerated].maxOre;  // Les planetes ne sont pas toutes initialement remplies
    randIndex ++;
    planets[nbEntityGenerated].regenerationTime = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;  // En millisecondes
    randIndex ++;

    int r = 150 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 50;
    randIndex ++;
    planets[nbEntityGenerated].radius = r;
    planets[nbEntityGenerated].x = x;
    planets[nbEntityGenerated].y = y;

    // Generation des planetes
    for (int j = nbEntityGenerated + 1; j < nbEntityGenerated + nbEntityNewSS; j++) {
        planets[j].id = j;
        planets[j].planetType = PLANET;
        planets[j].maxOre = 1 + generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 1000;  // Les planetes ne sont pas toutes rentables
        randIndex ++;
        planets[j].currentOre = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % (int)planets[j].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
        randIndex ++;
        planets[j].regenerationTime = generateRandNb32(currentSeed, nbEntityGenerated + randIndex) % 10000;  // En millisecondes
        randIndex ++;

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
