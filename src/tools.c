#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *supprElemList(void *list, int *nb_elem, int type_size, int i) {
    // Vérification des paramètres
    if (*nb_elem <= 0) {
        printf("Erreur [supprElemList] : liste vide ou nombre d'éléments incorrect.\n");
        return NULL;
    }
    if (i < 0 || i >= *nb_elem) {
        printf("Erreur [supprElemList] : indice invalide.\n");
        return NULL;
    }

    // Allocation de la nouvelle liste avec un élément en moins
    void *res = malloc((*nb_elem - 1) * type_size);
    if (!res) {
        printf("Erreur [supprElemList] : échec de l'allocation mémoire.\n");
        return NULL;
    }

    // Copier les éléments en ignorant celui à supprimer
    for (int j = 0, k = 0; j < *nb_elem; j++) {
        if (j != i) {
            memcpy((char *)res + k * type_size, (char *)list + j * type_size, type_size);
            k++;
        }
    }

    // Mettre à jour le nombre d'éléments
    (*nb_elem)--;

    // Libérer l'ancienne liste
    free(list);

    return res;
}