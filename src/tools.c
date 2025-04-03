#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
