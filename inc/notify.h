#ifndef NOTIFY_H
#define NOTIFY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Petites notifications ("toasts") affichees en bas de l'ecran.
 * Donne du retour visuel aux evenements (combat, construction, objectifs...).
 */

#define MAX_NOTIFICATIONS 5

void initNotify(TTF_Font *font);
void pushNotification(const char *text, SDL_Color color);
void updateNotifications(void);
void displayNotifications(void);
void destroyNotify(void);

#endif
