#pragma once
#include "splashkit.h"

#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"
#include "../Block/Block.h"
#include "../WorldConfig/WorldConfig.h"

#include <vector>

class World
{
    public:
        int width, height;
        unsigned int seed;
        int zoom;

        std::vector<std::vector<Block>> blocks;

        WorldConfig config;
        
        Noise noise_surface;

        int surface_base;

        std::vector<BiomeType> biome_map;

        Block get_block_at(int x, int y);

        const int BLOCK_SIZE = 2;

        std::vector<int> surface_map;

        World(int w, int h, unsigned int s, int z, const WorldConfig& cfg);

        int get_surface_height(int x);
        void generate();
        void draw();
};