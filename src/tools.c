#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include "camera.h"
#include "ship.h"
#include "planet.h"
#include "config.h"
#include "tools.h"


void *supprElemList(void *list, int *nb_elem, int type_size, int i) {
    // Vérification des paramètres
    if (*nb_elem <= 0 || i < 0 || i >= *nb_elem) {
        printf("Erreur [supprElemList] : paramètre invalide.\n");
        return NULL;
    }

    // Décaler les éléments après l'élément à supprimer
    char *ptr = (char *)list;
    memmove(ptr + i * type_size, ptr + (i + 1) * type_size, (*nb_elem - i - 1) * type_size);

    // Mettre à jour le nombre d'éléments
    (*nb_elem)--;

    // Réallouer la mémoire pour ajuster la taille
    void *res = realloc(list, (*nb_elem) * type_size);
    if (!res) {
        printf("Erreur [supprElemList] : échec du réajustement mémoire.\n");
        return list;  // Retourne l'ancienne liste si realloc échoue
    }

    return res;
}

// Fonction générique pour échanger deux éléments dans une liste
void *swapElements(void *list, int nb_elem, int type_size, int i, int j) {
    // Vérification des indices
    if (i < 0 || j < 0 || i >= nb_elem || j >= nb_elem) {
        printf("Erreur [swapElements] : indices invalides.\n");
        return NULL;
    }

    // Si les indices sont identiques, aucun échange n'est nécessaire
    if (i == j) return list;

    // Allocation d'un espace temporaire pour l'échange
    void *temp = malloc(type_size);
    if (!temp) {
        printf("Erreur [swapElements] : échec de l'allocation mémoire.\n");
        return NULL;
    }

    // Pointeurs vers les éléments à échanger
    char *ptr = (char *)list;
    void *elem1 = ptr + i * type_size;
    void *elem2 = ptr + j * type_size;

    // Échange des éléments
    memcpy(temp, elem1, type_size);
    memcpy(elem1, elem2, type_size);
    memcpy(elem2, temp, type_size);

    // Libérer la mémoire temporaire
    free(temp);

    return list;
}

// Fonction pour déplacer un élément à la fin de la liste
void *moveToEnd(void *list, int nb_elem, int type_size, int index) {
    // Vérification des indices
    if (index < 0 || index >= nb_elem) {
        printf("Erreur [moveToEnd] : indice invalide.\n");
        return NULL;
    }

    // Si l'élément est déjà à la fin, rien à faire
    if (index == nb_elem - 1) return list;

    // Allocation d'un espace temporaire pour l'élément à déplacer
    void *temp = malloc(type_size);
    if (!temp) {
        printf("Erreur [moveToEnd] : échec de l'allocation mémoire.\n");
        return NULL;
    }

    // Pointeurs vers l'élément et la fin
    char *ptr = (char *)list;
    void *elem = ptr + index * type_size;

    // Sauvegarde de l'élément
    memcpy(temp, elem, type_size);

    // Décalage des éléments suivants vers la gauche
    memmove(ptr + index * type_size, ptr + (index + 1) * type_size, (nb_elem - index - 1) * type_size);

    // Placer l'élément sauvegardé à la fin
    memcpy(ptr + (nb_elem - 1) * type_size, temp, type_size);

    // Libérer la mémoire temporaire
    free(temp);

    return list;
}

float distanceShipPlanet(Ship ship, Planet planet) {
    return sqrt(carre(ship.x + ship.w / 2. - planet.x) + carre(ship.y + ship.h / 2. - planet.y)) - planet.radius;
}

float distancePlanetPlanet(Planet planet1, Planet planet2) {
    return sqrt(carre(planet1.x - planet2.x) + carre(planet1.y - planet2.y));
}

float distanceShipShip(Ship ship1, Ship ship2) {
    return sqrt(carre(ship1.x + ship1.w / 2. - (ship2.x + ship2.w / 2.)) + carre(ship1.y + ship1.h / 2. - (ship2.y + ship2.h / 2.)));
}

static char arrayOfSyllabes[405][5] = {0};  // MODIFIER '385' SI BESOIN
static int sizeArrayOfSyllabes = 405;  // Nombre de syllabes differentes

void initArrayOfSyllabes(void) {
    if (arrayOfSyllabes[0][0] != '\0') {  // Initialise une seule fois arrayOfSyllabes
        return;
    }

    // Initialisation
    int newSyllabe = 0;  // Pour remplir arrayOfSyllabes

    // Creation des tableau de voyelles et consonnes
    char vowelArray[6] = {'a', 'e', 'i', 'o', 'u', 'y'};
    char multiVowel[9][2] = {{'a','i'}, {'a','e'}, {'a','u'}, {'a','y'}, {'e','i'}, {'e','o'}, {'e','u'}, {'e','y'}, {'o','u'}};
    char consonantArray[20] = {'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z'};
    char multiConsonance[7][2] = {{'b','h'}, {'t','h'}, {'r','h'}, {'l','h'}, {'p', 'h'}, {'z', 'h'}, {'c', 'z'}};

    // Generation du tableau de syllabes
    for (int i = 0; i < 20; i++) {  // Syllabes du type 'consonne + voyelle'
        for (int j = 0; j < 6; j++) {
            arrayOfSyllabes[newSyllabe][0] = consonantArray[i];
            arrayOfSyllabes[newSyllabe][1] = vowelArray[j];
            newSyllabe ++;
        }
        for (int j = 0; j < 9; j++) {  // Syllabe de type consonne plus double voyelle
            arrayOfSyllabes[newSyllabe][0] = consonantArray[i];
            arrayOfSyllabes[newSyllabe][1] = multiVowel[j][0];
            arrayOfSyllabes[newSyllabe][2] = multiVowel[j][1];
            newSyllabe ++;
        }
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {  // Syllabes du type 'double consonne + voyelle'
            arrayOfSyllabes[newSyllabe][0] = multiConsonance[i][0];
            arrayOfSyllabes[newSyllabe][1] = multiConsonance[i][1];
            arrayOfSyllabes[newSyllabe][2] = vowelArray[j];
            newSyllabe ++;
        }
        for (int j = 0; j < 9; j++) {  // Syllabes du type 'double consonne + double voyelle'
            arrayOfSyllabes[newSyllabe][0] = multiConsonance[i][0];
            arrayOfSyllabes[newSyllabe][1] = multiConsonance[i][1];
            arrayOfSyllabes[newSyllabe][2] = multiVowel[j][0];
            arrayOfSyllabes[newSyllabe][3] = multiVowel[j][1];
            newSyllabe ++;
        }
    }
}

void generateRandomName(char randomString[64], unsigned int seed, int index) {
    // Creation du tableau de syllabes
    initArrayOfSyllabes();

    // Obtenir le nombre de syllabes dans le mot
    int minNbSyllabes = 2;
    int maxNbSyllabes = 4;
    int nbSyllabes = minNbSyllabes + generateRandNb8(seed, index) % (maxNbSyllabes - minNbSyllabes + 1);

    // Pour que le mot genere ne soit pas le meme que celui avec index - 1
    index = index * maxNbSyllabes;

    // Generation du nom
    strcpy(randomString, "");
    char syllabe[5] = {0};
    for (int i = 0; i < nbSyllabes; i++) {
        strcpy(syllabe, arrayOfSyllabes[generateRandNb32(seed, index + i) % sizeArrayOfSyllabes]);  // Generer une nouvelle syllabe
        strcat(randomString, syllabe);  // Ajouter une nouvelle syllabe
    }
}

uint32_t generateRandNb32(uint32_t seed, int index) {  // linear congruential generator (LCG), parametres de Marsaglia
    if (seed == 0U) {
        printf("La seed ne doit pas etre nulle pour le generateur aleatoire de Marsaglia\n");
        return 0U;
    }

    index += 1;  // La variable index ne doit pas être nulle
    for (int i = 0; i < index; i++) {
        seed *= 69069U;
    }

    return seed;  // De la forme : seed * 69069 ** (index + 1)
}

uint8_t generateRandNb8(uint32_t seed, int index) {  // Pour generer de petits entiers (LCG peu efficace dans les bits de poids faible... )
    return (uint8_t)(generateRandNb32(seed, index) >> 24);  // On ne garde que les bits de poids fort
}
