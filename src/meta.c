#include "meta.h"

#include "events.h"

#define PIRATE_BOUNTY 30
#define PIRATE_SCORE 100


static Meta meta;


void initMeta(void) {
    meta.credits = 0;
    meta.score = 0;
    meta.piratesKilled = 0;
    meta.mineralsMined = 0;
    meta.shipsBuilt = 0;
    meta.wave = 0;
    meta.victoryReached = 0;
    for (int i = 0; i < UPG_COUNT; i++) {
        meta.upgrades[i] = 0;
    }
}

Meta *getMeta(void) {
    return &meta;
}

void addCredits(int64_t n) {
    meta.credits += n;
    if (meta.credits < 0) meta.credits = 0;
}

int spendCredits(int64_t n) {
    if (meta.credits >= n) {
        meta.credits -= n;
        return 1;
    }
    return 0;
}

void addScore(int64_t n) {
    meta.score += n;
}

void registerPirateKill(void) {
    meta.piratesKilled++;
    addCredits((int64_t)(PIRATE_BOUNTY * eventBountyMultiplier()));
    addScore(PIRATE_SCORE);
}

void registerMined(int64_t amount) {
    if (amount > 0) {
        meta.mineralsMined += amount;
    }
}

int upgradeLevel(UpgradeType t) {
    if (t < 0 || t >= UPG_COUNT) return 0;
    return meta.upgrades[t];
}

int upgradeCost(UpgradeType t) {
    int lvl = upgradeLevel(t);
    // Cout croissant : 150, 300, 450, ...
    return 150 * (lvl + 1);
}

int buyUpgrade(UpgradeType t) {
    if (t < 0 || t >= UPG_COUNT) return 0;
    if (meta.upgrades[t] >= UPG_MAX_LEVEL) return 0;
    if (!spendCredits(upgradeCost(t))) return 0;
    meta.upgrades[t]++;
    return 1;
}

float miningMultiplier(void) {
    return 1.0f + 0.30f * meta.upgrades[UPG_MINING];
}

float towerRangeMultiplier(void) {
    return 1.0f + 0.15f * meta.upgrades[UPG_TOWER];
}

int towerDamageBonus(void) {
    return meta.upgrades[UPG_TOWER];  // +1 degat par niveau
}

float shipSpeedMultiplier(void) {
    return 1.0f + 0.12f * meta.upgrades[UPG_SHIP_SPEED];
}

int shipArmorBonus(void) {
    return 40 * meta.upgrades[UPG_SHIP_ARMOR];
}
