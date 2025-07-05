#include "tile.h"

#include <stdint.h>
#include "camera.h"
#include "config.h"
#include "tools.h"
#include "renderer.h"


static uint8_t *tilesMatrix;  // Chaque bit représente un booléen
static Uint32 lastTileUpdateTime = 0;
static int byteCount;  // Nombre d'octets necessaires pour allouer le tableau tilesMatrix

void initTiles(void) {
    // Initialise la variable static 'byteCount'
    setByteCount();

    // Allocation du tableau de tuiles
    tilesMatrix = malloc(byteCount * sizeof(uint8_t));

    // Remplissage du tableau de tuiles
    memset(tilesMatrix, 0, byteCount * sizeof(uint8_t));  // Initialise tout le tableau a 0
    for (int i = 0; i < NUMBER_OF_HEXAGON_PER_HEIGHT; i++) {
        for (int j = 0; j < NUMBER_OF_HEXAGON_PER_WIDTH; j++) {
            setBit(i, j, 1);
        }
    }
}

uint8_t **getTilesMatrix(void) {
    return &tilesMatrix;
}

int *getByteCount(void) {
    return &byteCount;
}

void setByteCount(void) {
    int bitCount = (NUMBER_OF_HEXAGON_PER_WIDTH + 1) * (NUMBER_OF_HEXAGON_PER_HEIGHT + 1);
    byteCount = (bitCount + 7) / 8;  // Arrondi vers le haut
}


void displayTiles(SDL_Texture ***imageTextures) {
    if (getCameraScale() < LIMIT_UNZOOM) {  // Affichage des tuiles ssi l'on n'a pas trop dezoome
        return;
    }

    static int antiGapBetweenTiles = 15;
    SDL_Rect tileImgRect = {0, 0, (SIZE_HEXAGON * 2 + antiGapBetweenTiles * 2) * getCameraScale(), (SIZE_HEXAGON * SQRT3 + antiGapBetweenTiles * 2) * getCameraScale()};
    for (int i = (getCameraRect().y + (1 - 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / (SIZE_HEXAGON * SQRT3); 
         i < (getCameraRect().y + (1 + 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / (SIZE_HEXAGON * SQRT3) + 0.5; 
         i++) {
        for (int j = (getCameraRect().x + (1 - 1 / getCameraScale()) * SCREEN_WIDTH / 2.f ) / (SIZE_HEXAGON * 1.5); 
             j < (getCameraRect().x + SCREEN_WIDTH * 1.5f / getCameraScale()) / (SIZE_HEXAGON * 1.5) + 1; 
             j++) {
            tileImgRect.x = (SIZE_HEXAGON * 1.5 * j - SIZE_HEXAGON - getCameraRect().x - SCREEN_WIDTH / 2.f - antiGapBetweenTiles) * getCameraScale() + SCREEN_WIDTH / 2.f;  // Le -5 perdu au milieu : contre les approximations et ecarts entre tuiles
            tileImgRect.y = (SIZE_HEXAGON * SQRT3 * (i + 0.5 * ((j % 2 == 0) ? 0 : 1)) - SIZE_HEXAGON * SQRT3 / 2 - getCameraRect().y - SCREEN_HEIGHT / 2.f - antiGapBetweenTiles) * getCameraScale() + SCREEN_HEIGHT / 2.f;

            if (getBit(i, j) == 1) {
                SDL_RenderCopy(renderer, imageTextures[3][11], NULL, &tileImgRect);

            }
        }
    }
}

void updateTiles(Ship *ships, int shipCount) {
    if (SDL_GetTicks() < lastTileUpdateTime + REFRESH_TIME_TILE) {
        return;
    }

    lastTileUpdateTime = SDL_GetTicks();

    int i, j;  // Numero de la tuile ou est la fusee

    for (int k = 0; k < shipCount; k++) {
        j = (ships[k].x + ships[k].w / 2 + SIZE_HEXAGON * 0.75) / (SIZE_HEXAGON * 1.5);
        i = (ships[k].y + ships[k].h / 2 + SIZE_HEXAGON * SQRT3 / 2.f * ((j % 2 == 0) ? 1 : 0)) / (SIZE_HEXAGON * SQRT3);
        if (getBit(i, j) == 1) {
            setBit(i, j, 0);
        }
    }
}

void setBit(int i, int j, int value) {  // Raisonnemer matriciellement pour i et j
    int index = j * NUMBER_OF_HEXAGON_PER_HEIGHT + i;
    size_t byteIndex = index / 8;
    size_t bitIndex = index % 8;

    if (value)
        tilesMatrix[byteIndex] |= (1 << bitIndex);
    else
        tilesMatrix[byteIndex] &= ~(1 << bitIndex);
}

int getBit(int i, int j) {  // Raisonnemer matriciellement pour i et j
    int index = j * NUMBER_OF_HEXAGON_PER_HEIGHT + i;
    size_t byteIndex = index / 8;
    size_t bitIndex = index % 8;

    return (tilesMatrix[byteIndex] >> bitIndex) & 1;
}

void destroyTiles(void) {
    free(tilesMatrix);
}
