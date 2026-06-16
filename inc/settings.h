#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL2/SDL_mixer.h>

/* Centralise les reglages audio du jeu (musique, effets, sourdine).
 * Les canaux mixer sont organises ainsi :
 *   - canal 0 : musique de fond (en boucle)
 *   - canal 1 : effets d'interface
 *   - canaux 2+ : effets de combat (lasers, explosions)
 */

void initSettings(void);

void toggleMute(Mix_Chunk **sounds);   // Coupe/retablit tout le son
int  isMuted(void);

void toggleMusic(Mix_Chunk **sounds);  // Active/coupe uniquement la musique
int  isMusicOn(void);

void toggleSfx(void);                   // Active/coupe uniquement les effets
int  isSfxOn(void);

void applyAudioSettings(Mix_Chunk **sounds);  // Reapplique les volumes courants

#endif
