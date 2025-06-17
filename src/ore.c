#include "ore.h"
#include "planet.h"

int total_ore[ORE_TYPE_COUNT];

int getTotalOreWithInt(int i) {
    return total_ore[i];
}

int getTotalOre(Planet *planets, int planet_count, Ore ore_type) {
    int res = 0;
    for (int i=0; i<planet_count; i++) {
        for (int build_type = 0; build_type<BUILD_TYPE_COUNT; build_type++) {
            if (planets[i].builds[build_type].type == ORE_STORE || planets[i].builds[build_type].tank.ore == ore_type) {
                res += planets[i].builds->tank.currentCapacity;
            }
        }
    }
    return res;
}

void updateTotalOre(Planet *planets, int planet_count) {
    total_ore[0] = getTotalOre(planets, planet_count, FUEL);
    total_ore[1] = getTotalOre(planets, planet_count, ORE1);
    total_ore[2] = getTotalOre(planets, planet_count, ORE2);
    total_ore[3] = getTotalOre(planets, planet_count, ORE3);
    total_ore[4] = getTotalOre(planets, planet_count, ORE4);
}