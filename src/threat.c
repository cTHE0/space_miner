#include "threat.h"

#include <SDL2/SDL.h>


static Uint32 startTime = 0;
static int level = 0;
static float factor = 1.0f;


void initThreat(void) {
    startTime = SDL_GetTicks();
    level = 0;
    factor = 1.0f;
}

void updateThreat(long score) {
    int byTime = (SDL_GetTicks() - startTime) / 75000;  // +1 niveau toutes les 75 s
    int byScore = (int)(score / 4000);                  // +1 niveau tous les 4000 points

    level = byTime + byScore;
    if (level > THREAT_MAX) level = THREAT_MAX;
    if (level < 0) level = 0;

    factor = 1.0f + 0.20f * level;
}

int threatLevel(void) {
    return level;
}

float threatFactor(void) {
    return factor;
}

int enemyDamage(void) {
    return 1 + level / 3;  // Les pirates frappent plus fort en montant en menace
}
