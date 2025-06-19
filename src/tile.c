#include "tile.h"

#include <stdint.h>
#include "camera.h"
#include "config.h"
#include "tools.h"


static uint8_t *bitArray;  // Chaque bit représente un booléen


void initTiles(void) {
    int bitCount = (NUMBER_OF_HEXAGON_PER_WIDTH + 1) * (NUMBER_OF_HEXAGON_PER_HEIGHT + 1);
    int byteCount = (bitCount + 7) / 8;  // arrondi vers le haut

    // Allocation du tableau de tuiles
    bitArray = malloc(byteCount * sizeof(uint8_t));

    // Remplissage du tableau de tuiles
    for (int i = 0; i < NUMBER_OF_HEXAGON_PER_HEIGHT; i++) {
        for (int j = 0; j < NUMBER_OF_HEXAGON_PER_WIDTH; j++) {
            setBit(bitArray, j * NUMBER_OF_HEXAGON_PER_HEIGHT + i, 1);
        }
    }

    // for (int i = 0; i < NUMBER_OF_HEXAGON_PER_HEIGHT; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         setBit(bitArray, j * NUMBER_OF_HEXAGON_PER_HEIGHT + i, 1);
    //     }
    // }
}

void displayTiles(void) {
    if (getCameraScale() < LIMIT_UNZOOM) {  // Affichage des tuiles ssi l'on n'a pas trop dezoome
        return;
    }

    SDL_Point centerHexagon;
    for (int i = (getCameraRect().y + (1 - 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / SIZE_HEXAGON / sqrt(3); 
         i < (getCameraRect().y + (1 + 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / SIZE_HEXAGON / sqrt(3) + 0.5; 
         i++) {
        for (int j = (getCameraRect().x + (1 - 1 / getCameraScale()) * SCREEN_WIDTH / 2.f ) / SIZE_HEXAGON / 1.5; 
             j < (getCameraRect().x + SCREEN_WIDTH * 1.5f / getCameraScale()) / SIZE_HEXAGON / 1.5; 
             j++) {
            centerHexagon.x = (SIZE_HEXAGON * 1.5 * j - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
            centerHexagon.y = (SIZE_HEXAGON * sqrt(3) * (i + 0.5 * ((j % 2 == 0) ? 0 : 1)) - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;

            if (getBit(bitArray, j * NUMBER_OF_HEXAGON_PER_HEIGHT + i) == 1) {
                drawHexagon(centerHexagon, SIZE_HEXAGON * getCameraScale());
            }
        }
    }
}

void updateTiles(Ship *ships, int shipCount) {
    int i, j;  // Numero de la tuile ou est la fusee

    for (int k = 0; k < shipCount; k++) {
        j = (ships[k].x + ships[k].w / 2 + SIZE_HEXAGON) / (SIZE_HEXAGON * 1.5);
        i = (ships[k].y + ships[k].h / 2 + SIZE_HEXAGON * SQRT3 / 2.f * ((j % 2 == 0) ? 1 : 0)) / (SIZE_HEXAGON * SQRT3);
        if (getBit(bitArray, j * NUMBER_OF_HEXAGON_PER_HEIGHT + i) == 1) {
            setBit(bitArray, j * NUMBER_OF_HEXAGON_PER_HEIGHT + i, 0);
        }
    }
}

void setBit(uint8_t *array, int index, int value) {  // Set a bit to true or false
    size_t byteIndex = index / 8;
    size_t bitIndex = index % 8;

    if (value)
        array[byteIndex] |= (1 << bitIndex);
    else
        array[byteIndex] &= ~(1 << bitIndex);
}

int getBit(uint8_t *array, int index) {  // Get the boolean value at a specific index
    size_t byteIndex = index / 8;
    size_t bitIndex = index % 8;

    return (array[byteIndex] >> bitIndex) & 1;
}

void destroyTiles(void) {
    free(bitArray);
}
