#include "ore.h"
#include "planet.h"

int total_ore[ORE_TYPE_COUNT];

int getTotalOreWithInt(int i) {
    return total_ore[i];
}

int getTotalOre(Planet *planets, int planet_count, Ore ore_type) {
    int res = 0;
    for (int i=0; i<planet_count; i++) {
        for (int build_type; build_type<BUILD_TYPE_COUNT; build_type++) {
            if (planets[i].builds[build_type].type == ORE_STORE || planets[i].builds[build_type].tank.ore == ore_type) {
                printf("%d \n", planets[i].builds->tank.currentCapacity);
                res += planets[i].builds->tank.currentCapacity;
            }
        }
    }
    return res;
}

void updateTotalOre(Planet *planets, int planet_count) {
    getTotalOre(planets, planet_count, FUEL);
    getTotalOre(planets, planet_count, ORE1);
    getTotalOre(planets, planet_count, ORE2);
    getTotalOre(planets, planet_count, ORE3);
    getTotalOre(planets, planet_count, ORE4);
}