#include "planet.h"
#include "config.h"

void generateSolarSystem(int x, int y, int nbEntityGenerated, int nbEntityNewSS) { 
    // Initialisation du soleil
    planets[nbEntityGenerated].id = nbEntityGenerated;
    planets[nbEntityGenerated].planetType = SUN;
    planets[nbEntityGenerated].maxOre = 1 + rand () % 1000;  // Les planetes ne sont pas toutes rentables
    planets[nbEntityGenerated].currentOre = rand () % (int)planets[nbEntityGenerated].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
    planets[nbEntityGenerated].regenerationTime = rand () % 10000;  // En millisecondes

    int r = 150 + rand() % 50;
    planets[nbEntityGenerated].radius = r;
    planets[nbEntityGenerated].x = x;
    planets[nbEntityGenerated].y = y;

    // Generation des planetes
    for (int j = nbEntityGenerated + 1; j < nbEntityGenerated + nbEntityNewSS; j++) {
        planets[j].id = j;
        planets[j].planetType = PLANET;
        planets[j].maxOre = 1 + rand () % 1000;  // Les planetes ne sont pas toutes rentables
        planets[j].currentOre = rand () % (int)planets[j].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
        planets[j].regenerationTime = rand () % 10000;  // En millisecondes

        // Recherche d'une nouvelle planete eloignee de toutes les autres
        short planetIsAlone = 0;  // 0: la nouvelle planete est proche d'une autre, 1: la nouvelle planete est eloignee
        int rp, xp, yp;
        while (planetIsAlone == 0) {
            rp = 25 + rand() % 30;
            xp = x + (2000 - rand()%4000); //abscisse planète
            yp = y + (2000 - rand()%4000);
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