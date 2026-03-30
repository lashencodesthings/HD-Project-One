#pragma once

#include <vector>
#include "../Block/Block.h"
#include "splashkit.h"

//Configure the structs for the configuration
struct TerrainSnakesConfig {
    int VEIN_SPACING;
    int VEINS_PER_CLUSTER;
    int VEIN_COUNT_VARIATION;
    int MIN_VEIN_LENGTH;
    int VEIN_LENGTH_VARIATION;
    int MIN_BRUSH_RADIUS;
    int BRUSH_RADIUS_VARIATION;
    int MOVE_RANGE;
    int MOVE_OFFSET;
    double STEP_SCALE;
    int SURFACE_OFFSET;
    int BRANCH_PROBABILITY;
    int MAX_BRANCHES;    
};

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
    TerrainSnakesConfig terrain_snakes;
    std::vector<LayerConfig> layers;
};

//Function for loading the world data from a configuration json file
WorldConfig load_world_config(const string& path);