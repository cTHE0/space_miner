#include "assets_gestion.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <dirent.h>
#include "renderer.h"


static int nbCategories;   // Initialisation
static int *nbPicturesPerCategory;  // Initialisation


SDL_Texture* IMG_LoadTextureWithAlpha(const char* filePath, Uint8 alpha) {
    // Charger directement la texture avec SDL_Image
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        printf("Erreur lors du chargement de la texture : %s\n", IMG_GetError());
        return NULL;
    }

    // Définir le mode de mélange pour gérer l'alpha
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
        printf("Erreur lors du réglage du mode de mélange : %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }

    // Appliquer la transparence
    if (SDL_SetTextureAlphaMod(texture, alpha) != 0) {
        printf("Erreur lors de la modification de l'alpha : %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }

    return texture;
}

SDL_Texture ***loadTextures(void) {
    /* Pour ajouter un image, il suffit de l'ajouter dans un dossier et de la renommer comme
     * il se doit.
     */

    // Obtention des variables necessaires a l'importation
    const char *path = "assets/img/";
    nbCategories = foldersNbFunction(path);  // Nombre de dossiers
    char **foldersPath = foldersPathFunction(path);  // Chemins des dossiers
    nbPicturesPerCategory = filesNbFunction(foldersPath);  // Nombre de fichiers par categorie

    // Construction de imageTextures
    SDL_Texture ***imageTextures = malloc(nbCategories * sizeof(SDL_Texture**));

    for (int i = 0; i < nbCategories; i++) {
        imageTextures[i] = malloc(nbPicturesPerCategory[i] * sizeof(SDL_Texture*));

        if (strcmp(foldersPath[i], "assets/img/others4/") == 0) {  // Traitement spécifique pour 'others'
            imageTextures[i][0] = IMG_LoadTexture(renderer, "assets/img/others4/0.png");
            imageTextures[i][1] = IMG_LoadTextureWithAlpha("assets/img/others4/1.png", 100);
            imageTextures[i][2] = IMG_LoadTexture(renderer, "assets/img/others4/2.png");
            imageTextures[i][3] = IMG_LoadTextureWithAlpha("assets/img/others4/3.png", 100);
        }

        // Chargement des textures pour chaque image dans chaque dossier
        for (int j = 0; j < nbPicturesPerCategory[i]; j++) {
            char pathPng[128] = {0};
            snprintf(pathPng, sizeof(pathPng), "%s%d.png", foldersPath[i], j);

            imageTextures[i][j] = IMG_LoadTexture(renderer, pathPng);
            if (!imageTextures[i][j]) {
                printf("Erreur chargement de la texture %s : %s\n", pathPng, IMG_GetError());
                return NULL;
            }
        }
    }

    // Libération des chemins des dossiers
    for (int i = 0; i < nbCategories; i++) {
        free(foldersPath[i]);
    }
    free(foldersPath);

    return imageTextures;
}



int foldersNbFunction(const char *path) {
    // Ouvrir le dossier
    DIR *folder = opendir(path);
    if (folder == NULL) {
        perror("Erreur lors de l'ouverture du dossier");
        return -1;  // Retourne -1 en cas d'erreur
    }

    // Compter le nombre de dossier
    struct dirent *entree;
    int indexFolder = 0;
    while ((entree = readdir(folder)) != NULL) {
        if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {  // Ignorer dossiers spécials
            if (entree->d_type == DT_DIR) {  // Si l'entrée est un répertoire, l'incrémenter
                indexFolder++;
            }
        }
    }

    // Fermer le dossier après lecture
    closedir(folder);

    return indexFolder;
}

char **foldersPathFunction(const char *path) {
    // Ouvrir le dossier
    DIR *folder = opendir(path);
    if (folder == NULL) {
        perror("Erreur lors de l'ouverture du dossier");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire toutes les entrées du dossier
    char **foldersName = malloc(nbCategories * sizeof(char *));  // Nombre de dossier a allouer 
    struct dirent *entree;
    int indexFolder = 0;
    while ((entree = readdir(folder)) != NULL) {
        if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {  // Ignorer les dossiers speciaux
            if (entree->d_type == DT_DIR) {  // Si l'entrée est un répertoire, l'ajouter au tableau
                foldersName[indexFolder] = malloc(128 * sizeof(char));   // 128 caracteres par chemin maximum
                strcpy(foldersName[indexFolder], entree->d_name);  // Copier le nom du dossier
                indexFolder++;
            }
        }
    }

    // Fermuture du dossier après lecture
    closedir(folder);

    // Trier les noms de dossiers par ordre alphabétique
    qsort(foldersName, indexFolder, sizeof(char *), compare);

    // Ajouter le debut du chemin assets/img/ avant le nom du dossier
    for (int i = 0; i < nbCategories; i++) {
        char pathFolder[128];
        strcpy(pathFolder, path);
        strcat(pathFolder, foldersName[i]);
        strcat(pathFolder, "/");
        strcpy(foldersName[i], pathFolder);
    }

    return foldersName;
}

int *filesNbFunction(char **foldersPath) {  // Creer une liste d'entiers. Chaque entier est le nombre de fichier dans une categorie d'image
    int *filesNbPerFolder = malloc(nbCategories * sizeof(int));
    struct dirent *entree;

    for (int i = 0; i < nbCategories; i++) {  // Pour chaque categorie
        // Ouvrir le sous-dossier
        DIR *folder = opendir(foldersPath[i]);
        if (folder == NULL) {
            perror("Erreur lors de l'ouverture du dossier");
            return NULL;  // Retourner NULL en cas d'erreur
        }

        // Compter le nombre d'image.
        int indexFile = 0;
        while ((entree = readdir(folder)) != NULL) {  
            if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {  // Ignorer "." et ".."
                if (entree->d_type == DT_REG) {  // Si l'entrée est un fichier, l'ajouter au tableau
                    indexFile++;
                }
            }
        }
        filesNbPerFolder[i] = indexFile;

        // Fermuture du dossier après lecture
        closedir(folder);
    }

    return filesNbPerFolder;
}

int compare(const void *a, const void *b) {  // Fonction de comparaison pour qsort
    const char **nomA = (const char **)a;
    const char **nomB = (const char **)b;
    return strcmp(*nomA, *nomB);
}

void destroyImageTextures(SDL_Texture ***imageTextures) {
    for (int i = 0; i < nbCategories; i++) {
        for (int j = 0; j < nbPicturesPerCategory[i]; j++) {
            SDL_DestroyTexture(imageTextures[i][j]);
        }
        free(imageTextures[i]);
    }
    
    free(imageTextures);
    free(nbPicturesPerCategory);
}