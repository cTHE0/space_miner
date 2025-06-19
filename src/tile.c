#include "tile.h"

#include <stdint.h>
#include "camera.h"
#include "config.h"
#include "tools.h"


static uint8_t *bitArray;  // Chaque bit représente un booléen


void initTiles(void) {
    int bitCount = (int)(carre(MAP_SIZE / SIZE_HEXAGON) / 2.598076211);
    int byteCount = ((bitCount + 7) / 8);  // arrondi vers le haut

    // Allocation du tableau de tuiles
    bitArray = malloc(byteCount * sizeof(uint8_t));

    // Remplissage du tableau de tuiles
    for (int i = 0; i < MAP_SIZE / (SIZE_HEXAGON * 2); i++) {
        for (int j = 0; j < MAP_SIZE / (SIZE_HEXAGON * SQRT3); j++) {
            setBit(bitArray, j * MAP_SIZE / (SIZE_HEXAGON * 2) + i, 1);
        }
    }
}

void displayTiles(void) {
    if (getCameraScale() < LIMIT_UNZOOM) {  // Affichage des tuiles ssi l'on n'a pas trop dezoome
        return;
    }

    SDL_Point centerHexagon;

    for (int i = (getCameraRect().x + (1 - 1 / getCameraScale()) * SCREEN_WIDTH / 2.f ) / SIZE_HEXAGON / 1.5; 
         i < (getCameraRect().x + SCREEN_WIDTH * 1.5f / getCameraScale()) / SIZE_HEXAGON / 1.5; 
         i++) {
        for (int j = (getCameraRect().y + (1 - 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / SIZE_HEXAGON / sqrt(3); 
             j < (getCameraRect().y + (1 + 1 / getCameraScale()) * SCREEN_HEIGHT / 2.f) / SIZE_HEXAGON / sqrt(3) + 0.5; 
             j++) {
            if (i % 2 == 0) {
                centerHexagon.x = (SIZE_HEXAGON * 1.5 * i - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
                centerHexagon.y = (SIZE_HEXAGON * sqrt(3) * j - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
            } else {
                centerHexagon.x = (SIZE_HEXAGON * 1.5 * i - getCameraRect().x - SCREEN_WIDTH / 2.f) * getCameraScale() + SCREEN_WIDTH / 2.f;
                centerHexagon.y = (SIZE_HEXAGON * sqrt(3) * (j - 0.5) - getCameraRect().y - SCREEN_HEIGHT / 2.f) * getCameraScale() + SCREEN_HEIGHT / 2.f;
            }
            if (getBit(bitArray, (int)(j * SCREEN_WIDTH / (SIZE_HEXAGON * 2.f )) + i) == 1) {
                drawHexagon(centerHexagon, SIZE_HEXAGON * getCameraScale());
            }
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

void destroyTuiles(void) {
    free(bitArray);
}
