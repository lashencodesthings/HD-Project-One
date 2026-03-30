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
    // Core World Data
    int width, height;
    unsigned int seed;
    int zoom;

    const int BLOCK_SIZE = 16;

    std::vector<std::vector<Block>> blocks;
    std::vector<int> surface_map;
    std::vector<std::vector<int>> layer_limits;
    std::vector<BiomeType> biome_map;

    WorldConfig config;

    // Noise Generators
    Noise noise_surface;
    Noise noise_biome;

    // Terrain Base
    int surface_base;

    // Constructor
    World(int w, int h, unsigned int s, int z, const WorldConfig& cfg);

    // Generation
    void generate();
    void generate_base_terrain();
    void generate_secondary_fill();
    void placement_pass();

    // Rendering
    void draw(float cam_x, float cam_y);

    // Terrain Queries
    int get_surface_height(int x);
    int get_layer_at(int x, int y);
    int get_layer_index_at(int x, int y);

    Block get_block_at(int x, int y, BiomeType b_type, const std::vector<int>& layer_limit);
    std::vector<BlockOption>* get_options_for_layer(Biome& biome, int layer_index);

    // Get Types
    WallType get_wall_type_for_biome(BiomeType type, int layer_index);
    BiomeType get_nearest_neighbour(int x, int& out_dist);
    bool is_solid(int x, int y);
    
    // Gameplay
    point_2d get_random_spawn_point();
    void remove_block(int x, int y);
};