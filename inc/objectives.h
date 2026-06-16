#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ship.h"
#include "planet.h"

/* Objectifs sequentiels qui guident le joueur et le recompensent en credits.
 * Donne une direction claire, surtout en debut de partie.
 */

void initObjectives(TTF_Font *font);
void updateObjectives(Ship *ships, int shipCount, Planet *planets, int planetCount);
void displayObjectiveHUD(void);
void destroyObjectives(void);

#endif
