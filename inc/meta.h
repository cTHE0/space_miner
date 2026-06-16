#ifndef META_H
#define META_H

#include <stdint.h>

/* Etat meta-progression du joueur : credits, score, statistiques et
 * ameliorations globales achetables au Centre de commandement.
 * Sauvegarde dans le fichier de backup (bloc ajoute a la fin).
 */

typedef enum {
    UPG_MINING,      // Rendement des mines
    UPG_TOWER,       // Puissance et portee des tours de defense
    UPG_SHIP_SPEED,  // Vitesse des nouvelles fusees
    UPG_SHIP_ARMOR,  // Blindage des nouvelles fusees
    UPG_COUNT
} UpgradeType;

#define UPG_MAX_LEVEL 8

typedef struct {
    int64_t credits;
    int64_t score;
    int piratesKilled;
    int64_t mineralsMined;
    int shipsBuilt;
    int wave;
    int upgrades[UPG_COUNT];
    int victoryReached;
    int objectiveIndex;
} Meta;

void initMeta(void);
Meta *getMeta(void);

void addCredits(int64_t n);
int  spendCredits(int64_t n);   // 1 si paye, 0 sinon
void addScore(int64_t n);

void registerPirateKill(void);
void registerMined(int64_t amount);

int  upgradeLevel(UpgradeType t);
int  upgradeCost(UpgradeType t);     // Cout du prochain niveau (credits)
int  buyUpgrade(UpgradeType t);      // 1 si achete

// Multiplicateurs/bonus derives des ameliorations
float miningMultiplier(void);
float towerRangeMultiplier(void);
int   towerDamageBonus(void);
float shipSpeedMultiplier(void);
int   shipArmorBonus(void);

#endif
