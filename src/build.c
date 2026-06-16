#include "build.h"

#include "planet.h"
#include "ship.h"
#include "renderer.h"
#include "camera.h"
#include "mine.h"
#include "config.h"
#include "meta.h"


static Uint32 lastBuildUpdateTime = 0;


void initBuildsPlanet(Planet *planet) {
	// Crée tous les bâtiments sur la planète sélectionnée
    for (int i = 0; i < BUILD_TYPE_COUNT; i++) {
        switch (i) {
            case 0:
                planet->builds[i].type = FACTORY;
                planet->builds[i].level = 0;
                planet->builds[i].price = (Price) {ORE1, 300};
                break;
            case 1:
                planet->builds[i].type = DEFENCE_TOWER;
                planet->builds[i].level = 0;
                planet->builds[i].damages = 0;
                planet->builds[i].price = (Price) {ORE1, 250};
                break;
            case 2:
            case 4:
            case 6:
            case 8:
            case 10: {
                Ore ore = i / 2 - 1;
                int built = (ore == FUEL || ore == ORE1);  // Seuls le fuel et le fer sont stockables au depart
                planet->builds[i].type = ORE_STORE;
                planet->builds[i].level = built;
                planet->builds[i].tank = (Compartment){ore, 0, 0, 0, 0, built ? STARTING_ORE_STOCK : 0, 10000, 0, built};
                // Le stockage du fuel/fer se paie en fer, l'or se paie en or, etc. (arbre technologique)
                planet->builds[i].price = (Price) {(ore <= ORE2) ? ORE1 : (Ore)(ore - 1), 130};
                break;
            }
            case 3:
            case 5:
            case 7:
            case 9:
            case 11: {
                Ore ore = i / 2 - 1;
                planet->builds[i].type = ORE_MINE;
                planet->builds[i].level = 0;
                planet->builds[i].mine = (Mine){ore, 1000};
                planet->builds[i].price = (Price) {(ore <= ORE2) ? ORE1 : (Ore)(ore - 1), 170};
                break;
            }
            default:
                break;
        }
    }
}

int getBuildCost(Build *build) {
    // Cout pour la prochaine action (construction si non construit, sinon amelioration)
    int base = build->price.price;
    if (build->level == 0) {
        return base;
    }
    return base + base * build->level;  // Le cout des ameliorations augmente avec le niveau
}

int planetOreStock(Planet *planet, Ore ore) {
    for (int i = 2; i < BUILD_TYPE_COUNT; i += 2) {
        if (planet->builds[i].type == ORE_STORE && planet->builds[i].tank.ore == ore && planet->builds[i].level > 0) {
            return (int)planet->builds[i].tank.currentCapacity;
        }
    }
    return 0;
}

int payOre(Planet *planet, Ore ore, int amount) {
    for (int i = 2; i < BUILD_TYPE_COUNT; i += 2) {
        if (planet->builds[i].type == ORE_STORE && planet->builds[i].tank.ore == ore && planet->builds[i].level > 0) {
            if (planet->builds[i].tank.currentCapacity >= amount) {
                planet->builds[i].tank.currentCapacity -= amount;
                return 1;
            }
            return 0;  // Stock insuffisant
        }
    }
    return 0;  // Pas de reservoir construit pour ce minerai
}

void ensureStoreBuilt(Planet *planet, Ore ore) {
    // Construit gratuitement le reservoir d'un minerai si la mine correspondante est posee
    for (int i = 2; i < BUILD_TYPE_COUNT; i += 2) {
        if (planet->builds[i].type == ORE_STORE && planet->builds[i].tank.ore == ore && planet->builds[i].level == 0) {
            planet->builds[i].level = 1;
            planet->builds[i].tank.level = 1;
            return;
        }
    }
}

void updateBuilds(Planet *planets, int planetCount) {
    if (SDL_GetTicks() < lastBuildUpdateTime + REFRESH_TIME_BUILDS) {
        return;
    }

    lastBuildUpdateTime = SDL_GetTicks();

    for (int i = 0; i < planetCount; i++) {
        for (int j = 0; j < BUILD_TYPE_COUNT; j++) {
            if (planets[i].builds[j].type == NOTHING || planets[i].builds[j].level == 0) {
                continue;
            } 

            switch (planets[i].builds[j].type) {
                case ORE_STORE:  // Tout est gere dans ship.c
                    break;

                case ORE_MINE:
                    updateBuildMine(planets[i].builds, &planets[i].builds[j].mine, planets[i].abundance[planets[i].builds[j].mine.ore]);
                    break;

                case DEFENCE_TOWER:
                    break;

                case FACTORY:
                    break;

                default:
                    break;
            }
        }
    }
}

void updateBuildMine(Build *builds, Mine *mine, int abundance) {
    for (int i = 0; i < ORE_TYPE_COUNT; i++) {
        if (builds[2 * i + 2].type == ORE_STORE && builds[2 * i + 2].tank.ore == mine->ore && builds[2 * i + 2].level > 0) {  // Si l'on a trouve le bon reservoir construit, acceuillant les bons minerais...
            float produced = mine->productivity * (abundance / 100.f) * miningMultiplier();
            int before = builds[2 * i + 2].tank.currentCapacity;
            builds[2 * i + 2].tank.currentCapacity += produced;

            if (builds[2 * i + 2].tank.currentCapacity > builds[2 * i + 2].tank.maxCapacity) {
                builds[2 * i + 2].tank.currentCapacity = builds[2 * i + 2].tank.maxCapacity;
            }
            registerMined(builds[2 * i + 2].tank.currentCapacity - before);  // Comptabilise l'extraction reelle
            return;
        }
    }

    // Aucun reservoir construit pour ce minerai : la production est perdue (le joueur doit construire le stockage)
    return;
}

void displayBuildsAroundPlanet(Planet *planet, int countBuild, SDL_Texture **imageTextures, SDL_Point texture_dimensions) {
    float scale = getCameraScale();

    // Taille affichée de la planète
    float screenRadius = 43 * planet->radius * scale / 40;

    // Le vrai centre affiché de la planète
    float centerX = (planet->x - getCameraRect().x - SCREEN_WIDTH / 2.0f) * scale + SCREEN_WIDTH / 2.0f;
    float centerY = (planet->y - getCameraRect().y - SCREEN_HEIGHT / 2.0f) * scale + SCREEN_HEIGHT / 2.0f;

    SDL_Point center = {texture_dimensions.x * scale / 2, texture_dimensions.y * scale / 2};

    for (int i = 0; i < countBuild; i++) {
        // Angle autour de la planète
        float angle = i * 2.0f * M_PI / countBuild;
        float angle_deg = angle * 180.0f / M_PI;

        // Position orbitale autour du centre
        float x = centerX + screenRadius * cosf(angle);
        float y = centerY + screenRadius * sinf(angle);

        // Rectangle destination
        SDL_Rect dest = {(int)(x - texture_dimensions.x * scale / 2),
                         (int)(y - texture_dimensions.y * scale / 2),
                         (int)(texture_dimensions.x * scale),
                         (int)(texture_dimensions.y * scale)
                        };

        // Tangente = angle + 90°
        float tangent_angle = angle_deg + 90.0f;

        SDL_RenderCopyEx(renderer, imageTextures[i], NULL, &dest, tangent_angle, &center, SDL_FLIP_NONE);
    }
}
