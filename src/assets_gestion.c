#include "assets_gestion.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <dirent.h>
#include <SDL2/SDL_mixer.h>
#include "renderer.h"
#include "ship_window.h"
#include "planet_window.h"
#include "basic_ship_window.h"
#include "info_view.h"
#include "pause_window.h"


static int nbCategories;   // Initialisation
static int *nbPicturesPerCategory;  // Initialisation


SDL_Texture* IMG_LoadTextureWithAlpha(const char* filePath, Uint8 alpha) {
    // Charger directement la texture avec SDL_Image
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        printf("Erreur lors du chargement de la texture : %s\n", IMG_GetError());
        return NULL;
    }

    // Definir le mode de melange pour gerer l'alpha
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
        printf("Erreur lors du reglage du mode de melange : %s\n", SDL_GetError());
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

        if (strcmp(foldersPath[i], "assets/img/_10_withAlpha/") == 0) {  // Traitement specifique pour pour les images transparentes
            // imageTextures[i][0] = IMG_LoadTextureWithAlpha("assets/img/_10_withAlpha/0.png", 100);
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

    // Liberation des chemins des dossiers
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
        if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {  // Ignorer dossiers specials
            if (entree->d_type == DT_DIR) {  // Si l'entree est un repertoire, l'incrementer
                indexFolder++;
            }
        }
    }

    // Fermer le dossier apres lecture
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

    // Lire toutes les entrees du dossier
    char **foldersName = malloc(nbCategories * sizeof(char *));  // Nombre de dossier a allouer 
    struct dirent *entree;
    int indexFolder = 0;
    while ((entree = readdir(folder)) != NULL) {
        if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {  // Ignorer les dossiers speciaux
            if (entree->d_type == DT_DIR) {  // Si l'entree est un repertoire, l'ajouter au tableau
                foldersName[indexFolder] = malloc(128 * sizeof(char));   // 128 caracteres par chemin maximum
                strcpy(foldersName[indexFolder], entree->d_name);  // Copier le nom du dossier
                indexFolder++;
            }
        }
    }

    // Fermuture du dossier apres lecture
    closedir(folder);

    // Trier les noms de dossiers par ordre alphabetique
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
                if (entree->d_type == DT_REG) {  // Si l'entree est un fichier, l'ajouter au tableau
                    indexFile++;
                }
            }
        }
        filesNbPerFolder[i] = indexFile;

        // Fermuture du dossier apres lecture
        closedir(folder);
    }

    return filesNbPerFolder;
}

int compare(const void *a, const void *b) {  // Fonction de comparaison pour qsort
    const char **nomA = (const char **)a;
    const char **nomB = (const char **)b;
    return strcmp(*nomA, *nomB);
}

void initRects(SDL_Texture **textTextures) {
    initShipWindowRects(textTextures);
    initPlanetWindowRects(textTextures);
    initBasicShipWindowRects(textTextures);
    initInfoViewRects(textTextures);
    initPauseWindowRects(textTextures);
}

void initSounds(Mix_Chunk ***sounds) {
    // Allouer 255 canaux pour jouer plusieurs sons en même temps
    int allocated = Mix_AllocateChannels(255);
    if (allocated < 255) {
        printf("Seulement %d canaux audio ont été alloués sur 255 demandés\n", allocated);
    }

    // Allocation du tableau de sons
    *sounds = malloc(SONGS_NUMBER * sizeof(Mix_Chunk*));

    // Chargement des effets sonores
    (*sounds)[0] = Mix_LoadWAV("assets/songs/20250613-184110.mp3");
    (*sounds)[1] = Mix_LoadWAV("assets/songs/20250613 173819.mp3");
    (*sounds)[2] = Mix_LoadWAV("assets/songs/mixkit-bonus-earned-in-video-game-2058.wav");
    (*sounds)[3] = Mix_LoadWAV("assets/songs/mixkit-martial-arts-fast-punch-2047.wav");
    (*sounds)[4] = Mix_LoadWAV("assets/songs/mixkit-mechanical-crate-pick-up-3154.wav");
    (*sounds)[5] = Mix_LoadWAV("assets/songs/mixkit-player-jumping-in-a-video-game-2043.wav");
    (*sounds)[6] = Mix_LoadWAV("assets/songs/mixkit-metallic-sword-strike-2160.wav");
    (*sounds)[7] = Mix_LoadWAV("assets/songs/mixkit-metal-arrow-fast-hit-2770.wav");
    (*sounds)[8] = Mix_LoadWAV("assets/songs/mixkit-wrong-answer-bass-buzzer-948.wav");
    (*sounds)[9] = Mix_LoadWAV("assets/songs/mixkit-sci-fi-positive-notification-266.wav");
    (*sounds)[10] = Mix_LoadWAV("assets/songs/mixkit-blow-breaking-the-air-2057.wav");
    (*sounds)[11] = Mix_LoadWAV("assets/songs/star_wars.wav");
    (*sounds)[12] = Mix_LoadWAV("assets/songs/explosion.wav");

    // Vérification des chargements
    for (int i = 0; i < SONGS_NUMBER; i++) {
        if ((*sounds)[i] == NULL) {
            printf("Erreur de chargement du son %d: %s\n", i, Mix_GetError());
            return;
        }
    }
}


void freeSongs(Mix_Chunk **sounds) {
    for (int i = 0; i < SONGS_NUMBER; i++) {
        Mix_FreeChunk(sounds[i]);
    }
    free(sounds);
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