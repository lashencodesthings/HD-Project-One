#pragma once
#include <vector>
#include <map>
#include "../Block/Block.h"

enum class BiomeType {
    Plains,
    Desert,
    Snow,
    Jungle
};

struct Biome {
    BlockType surface_block;
    BlockType subsurface_block;
    BlockType underground_block;
    BlockType cavern_block;
    BlockType underworld_block;

    WallType underground_wall;
    WallType cavern_wall;
    WallType underworld_wall;
};

extern std::map<BiomeType, Biome> biome_data;
extern std::vector<BiomeType> generate_biome_map(int width, unsigned int seed);