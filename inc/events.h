#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>

/* Evenements galactiques aleatoires : modificateurs temporaires qui
 * dynamisent la partie (boom minier, marche en folie, deferlante de
 * pirates, prime triplee...). Donne du rythme et de la rejouabilite.
 */

typedef enum {
    EVENT_NONE,
    EVENT_MINING_BOOM,
    EVENT_MARKET_BOOM,
    EVENT_PIRATE_SURGE,
    EVENT_BOUNTY_FRENZY,
    EVENT_COUNT
} EventType;

void initEvents(void);
void updateEvents(void);

EventType currentEvent(void);
Uint32 eventTimeLeftMs(void);
float eventProgress(void);   // 0..1 part du temps ecoule

float eventMiningMultiplier(void);
float eventMarketMultiplier(void);
float eventBountyMultiplier(void);
int   eventPirateSurge(void);

#endif
