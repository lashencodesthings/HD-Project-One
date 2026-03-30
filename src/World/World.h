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
        Noise noise_biome;

        int surface_base;

        std::vector<BiomeType> biome_map;

        Block get_block_at(int x, int y, BiomeType b_type, const std::vector<int>& layer_limit);

        const int BLOCK_SIZE = 2;

        std::vector<int> surface_map;

        World(int w, int h, unsigned int s, int z, const WorldConfig& cfg);

        int get_surface_height(int x);
        void generate();
        void draw();

        void placement_pass();

        int get_layer_at(int x, int y);
        
        WallType get_wall_type_for_biome(BiomeType type, int layer_index);
        BiomeType get_nearest_neighbour(int x, int& out_dist);

        void generate_voronoi_layer(int layer_index, const std::vector<BlockOption>& options);

        void generate_base_terrain();
        void generate_secondary_fill();
        int get_layer_index_at(int x, int y);
        
        std::vector<BlockOption>* get_options_for_layer(Biome& biome, int layer_index);
        std::vector<std::vector<int>> layer_limits;
};