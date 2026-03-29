#pragma once

#include <vector>
#include "../Block/Block.h"
#include "splashkit.h"

// Configure the structs for the configuration
struct TerrainConfig { 
    double amplitude_ratio;
    double base_height_ratio;
    double big_scale;
    double small_scale;
    double small_weight;
};

struct NoiseConfig {
    int surface_offset;
    int underground_offset;
};

struct LayerConfig {
    int thickness;
    int variation;
    double noise_scale;

    BlockType block;
    WallType wall;
};

struct WorldConfig {
    TerrainConfig terrain;
    NoiseConfig noise;
    std::vector<LayerConfig> layers;
};

// Function for loading the world data from a configuration json file
WorldConfig load_world_config(const string& path);