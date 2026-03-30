#pragma once
#include <vector>
#include <map>
#include "../Block/Block.h"
#include "../BlockOption/BlockOption.h"

enum class BiomeType {
    Plains = 0,
    Desert = 1,
    Snow = 2,
    Jungle = 3
};

struct Biome {
    std::vector<BlockOption> surface;
    std::vector<BlockOption> subsurface;
    std::vector<BlockOption> underground;
    std::vector<BlockOption> cavern;
    std::vector<BlockOption> underworld;

    WallType underground_wall;
    WallType cavern_wall;
    WallType underworld_wall;
};

extern std::map<BiomeType, Biome>& get_biome_data();
extern std::vector<BiomeType> generate_biome_map(int width, unsigned int seed);