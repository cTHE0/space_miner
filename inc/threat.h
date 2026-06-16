#ifndef THREAT_H
#define THREAT_H

/* Niveau de menace : difficulte croissante avec le temps et la reussite du
 * joueur. Plus la menace est elevee, plus les pirates sont nombreux,
 * resistants et dangereux. Coeur de la courbe de difficulte.
 */

void initThreat(void);
void updateThreat(long score);

int   threatLevel(void);     // 0..THREAT_MAX
float threatFactor(void);    // multiplicateur de force des pirates
int   enemyDamage(void);     // degats d'un tir de pirate

#define THREAT_MAX 20

#endif
