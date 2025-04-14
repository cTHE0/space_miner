#include "planet.h"
#include "config.h"

void generateSolarSystem(Planet **planets, int x, int y, int i, int nb_planets_ss) { 
    // i : emplacement de la planète suivante à crée
    // nb_planets_ss : nb de planètes à créer ds ce système solaire (soleil non compris)

    (*planets)[i].idPicture = 9;
    (*planets)[i].maxOre = 1 + rand () % 1000;  // Les planetes ne sont pas toutes rentables
    (*planets)[i].currentOre = rand () % (int)(*planets)[i].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
    (*planets)[i].regenerationTime = rand () % 10000;  // En millisecondes
    int r;
    r = 150 + rand() % 50;
    (*planets)[i].radius = r;
    (*planets)[i].x = x;
    (*planets)[i].y = y;

    for (int j = i+1; j <= i+nb_planets_ss; j++) {
        (*planets)[j].idPicture = rand () % 10 + 1;
        (*planets)[j].maxOre = 1 + rand () % 1000;  // Les planetes ne sont pas toutes rentables
        (*planets)[j].currentOre = rand () % (int)(*planets)[j].maxOre;  // Les planetes ne sont pas toutes remplies de ressources
        (*planets)[j].regenerationTime = rand () % 10000;  // En millisecondes

        // Recherche d'une nouvelle planete eloignee de toutes les autres
        short planetIsAlone = 0;  // 0: la nouvelle planete est proche d'une autre, 1: la nouvelle planete est eloignee
        int rp, xp, yp;
        while (planetIsAlone == 0) {
            rp = 25 + rand() % 30;
            xp = x + (2000 - rand()%4000); //abscisse planète
            yp = y + (2000 - rand()%4000);
            planetIsAlone = 1;  // La nouvelle planete est eloignee A PART SI l'on en detecte une autre a cote

            for (int k = i; k < j; k++) {  // Verifie qu'il n'y ait pas de planetes trop proches de la planete i
                if (planetIsAlone == 1 && 
                    (abs(xp - (int)(*planets)[k].x) < rp + (*planets)[k].radius &&
                     abs(yp - (int)(*planets)[k].y) < rp + (*planets)[k].radius)) {
                    planetIsAlone = 0;
                    break;
                }
            }
        }
        (*planets)[j].radius = rp;
        (*planets)[j].x = xp;
        (*planets)[j].y = yp;
    }
}