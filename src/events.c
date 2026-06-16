#include "events.h"

#include <stdlib.h>
#include "notify.h"
#include "assets_gestion.h"

#define EVENT_MIN_GAP 35000u   // Delai min avant un nouvel evenement (ms)
#define EVENT_MAX_GAP 60000u   // Delai max
#define EVENT_DURATION 24000u  // Duree d'un evenement (ms)
#define FIRST_EVENT_DELAY 30000u


static EventType activeEvent = EVENT_NONE;
static Uint32 eventStart = 0;
static Uint32 nextEventTime = 0;


void initEvents(void) {
    activeEvent = EVENT_NONE;
    eventStart = 0;
    nextEventTime = 0;  // Programme au premier update
}

static void announce(EventType e) {
    switch (e) {
        case EVENT_MINING_BOOM:   pushNotification("GALACTIC EVENT: Mining boom! (x2 yield)", GREEN2); break;
        case EVENT_MARKET_BOOM:   pushNotification("GALACTIC EVENT: Market boom! (x2 sell price)", GREEN2); break;
        case EVENT_PIRATE_SURGE:  pushNotification("GALACTIC EVENT: Pirate surge incoming!", RED); break;
        case EVENT_BOUNTY_FRENZY: pushNotification("GALACTIC EVENT: Bounty frenzy! (x3 bounties)", (SDL_Color){255, 200, 0, 255}); break;
        default: break;
    }
}

void updateEvents(void) {
    Uint32 now = SDL_GetTicks();

    // Programmation initiale
    if (nextEventTime == 0) {
        nextEventTime = now + FIRST_EVENT_DELAY;
        return;
    }

    // Fin de l'evenement courant
    if (activeEvent != EVENT_NONE && now - eventStart >= EVENT_DURATION) {
        activeEvent = EVENT_NONE;
        nextEventTime = now + EVENT_MIN_GAP + rand() % (EVENT_MAX_GAP - EVENT_MIN_GAP);
        pushNotification("Galactic event ended.", WHITE);
        return;
    }

    // Declenchement d'un nouvel evenement
    if (activeEvent == EVENT_NONE && now >= nextEventTime) {
        activeEvent = (EventType)(1 + rand() % (EVENT_COUNT - 1));
        eventStart = now;
        announce(activeEvent);
    }
}

EventType currentEvent(void) {
    return activeEvent;
}

Uint32 eventTimeLeftMs(void) {
    if (activeEvent == EVENT_NONE) return 0;
    Uint32 elapsed = SDL_GetTicks() - eventStart;
    return (elapsed >= EVENT_DURATION) ? 0 : (EVENT_DURATION - elapsed);
}

float eventProgress(void) {
    if (activeEvent == EVENT_NONE) return 0.f;
    float p = (SDL_GetTicks() - eventStart) / (float)EVENT_DURATION;
    return (p > 1.f) ? 1.f : p;
}

float eventMiningMultiplier(void) {
    return (activeEvent == EVENT_MINING_BOOM) ? 2.0f : 1.0f;
}

float eventMarketMultiplier(void) {
    return (activeEvent == EVENT_MARKET_BOOM) ? 2.0f : 1.0f;
}

float eventBountyMultiplier(void) {
    return (activeEvent == EVENT_BOUNTY_FRENZY) ? 3.0f : 1.0f;
}

int eventPirateSurge(void) {
    return activeEvent == EVENT_PIRATE_SURGE;
}
