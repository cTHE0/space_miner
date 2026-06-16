#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL2/SDL.h>
#include "ship.h"
#include "planet.h"

/* Minimap dans le coin inferieur droit : vue d'ensemble de la galaxie
 * (soleils, planetes, flotte, pirates) + cadre de la camera.
 * Un clic recentre la camera sur la zone visee.
 */

void displayMinimap(Ship *ships, int shipCount, Planet *planets, int planetCount);
int minimapClick(SDL_Point mouse);  // 1 si le clic a ete consomme (recentrage)

#endif
