#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "planet.h"
#include "ship.h"

/* Centre de commandement : vente de minerais contre des credits et achat
 * d'ameliorations globales permanentes. Coeur de la meta-progression.
 */

void initCommandWindowRects(void);
void displayCommandWindow(SDL_Texture ***imageTextures, SDL_Texture **textTextures);
void commandWindowGestion(Mix_Chunk **sounds, Ship **ships, int *shipCount, Planet *planets, int planetCount, SDL_Point mouse);

#endif
