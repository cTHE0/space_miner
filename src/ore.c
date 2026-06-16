#include "ore.h"
#include "planet.h"


static int totalOre[ORE_TYPE_COUNT];


int getTotalOreWithInt(int i) {
    return totalOre[i];
}

int getTotalOre(Planet *planets, int planetCount, Ore oreType) {
    int res = 0;
    for (int i = 0; i < planetCount; i++) {
        for (int buildType = 0; buildType < BUILD_TYPE_COUNT; buildType++) {
            if (planets[i].builds[buildType].type == ORE_STORE && planets[i].builds[buildType].tank.ore == oreType && planets[i].builds[buildType].level > 0) {
                res += planets[i].builds[buildType].tank.currentCapacity;
            }
        }
    }
    
    return res;
}

int drainTotalOre(Planet *planets, int planetCount, Ore oreType, int amount) {
    // Retire jusqu'a 'amount' unites du minerai depuis les reservoirs construits ; renvoie la quantite reellement retiree
    int drained = 0;
    for (int i = 0; i < planetCount && drained < amount; i++) {
        for (int b = 0; b < BUILD_TYPE_COUNT && drained < amount; b++) {
            if (planets[i].builds[b].type == ORE_STORE && planets[i].builds[b].tank.ore == oreType && planets[i].builds[b].level > 0) {
                int take = amount - drained;
                if (take > planets[i].builds[b].tank.currentCapacity) {
                    take = planets[i].builds[b].tank.currentCapacity;
                }
                planets[i].builds[b].tank.currentCapacity -= take;
                drained += take;
            }
        }
    }
    return drained;
}

void updateTotalOre(Planet *planets, int planetCount) {
    totalOre[0] = getTotalOre(planets, planetCount, FUEL);
    totalOre[1] = getTotalOre(planets, planetCount, ORE1);
    totalOre[2] = getTotalOre(planets, planetCount, ORE2);
    totalOre[3] = getTotalOre(planets, planetCount, ORE3);
    totalOre[4] = getTotalOre(planets, planetCount, ORE4);
}
