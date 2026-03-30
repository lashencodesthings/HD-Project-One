#include "World.h"
#include "../Block/Block.h"
#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"
#include <iostream>

/*
    Author: Lashen Dharmadasa
    
    Description: 
        This system generates the 2D terrain for this game, using Perlin Noise.
        It has a lot of configuration variables, for my experimentation during this project.
        It's crude in that it completes the first and second pass for biomes and simple generation.
        Tunnelling and other features come later on.

    Parameters:
        1. Terrain Shape
            - amplitude_ratio
                Controls how tall terrain is are relative to the world height.

            - base_height_ratio
                The baseline surface height of the terrain.

            - big_scale
                Frequency of large terrain hills.
                A lower value provides wide and smooth hills.
                A Higher value produces rapid elevation changes in the terrain.

            - small_scale
                Frequency of fine detail noise like thin hills.

            - small_weight
                Influence of fine detail noise.
                A higher value provides rougher terrain.

        2. Terrain Noise Config
            I added offsets applied to the world seed to generate independent noise fields for each layer, as I felt having the same shape was not appealing.

            - surface_offset
                Used for terrain height offsets.

            - underground_offset
                Used for underground layer variation

        3. Seperate layer config
            - thickness
                Base thickness of the layer.

            - variation
                Amount of noise-based variation in thickness.

            - noise_scale
                Controls how frequently thickness of the layer changes across the world.

            - block
                Block used for this layer.

            - wall
                Background wall for this layer.
*/

// World Constructor
World::World(int w, int h, unsigned int s, int z, const WorldConfig& cfg) 
    : width(w), height(h), seed(s), zoom(z), config(cfg), noise_surface(s + cfg.noise.surface_offset), noise_biome(s + 2000)
{
    surface_base = height * config.terrain.base_height_ratio;
    blocks.assign(width, std::vector<Block>(height, Block(Air)));
}

// Main generation
void World::generate()
{
    biome_map = generate_biome_map(width, seed);
    surface_map.resize(width);
    warp_map.resize(width, std::vector<int>(height));
    layer_limits.resize(width);

    #pragma omp parallel for // Independant for loops so I parallelise it so it's running on a different thread
    for (int x = 0; x < width; x++) {
        surface_map[x] = get_surface_height(x);

        for (int y = 0; y < height; y++) {
            double warp1 = noise_biome.value({ x * 0.02, y * 0.05 }) * 10.0;
            double warp2 = noise_biome.value({ x * 0.05 + 100.0, y * 0.05 + 100.0 }) * 5.0;
            warp_map[x][y] = std::clamp(x + (int)(warp1 + warp2), 0, width - 1);
        }

        // Precompute layer limits
        int current_y = surface_map[x];
        layer_limits[x].resize(config.layers.size());
        for (size_t i = 0; i < config.layers.size(); i++) {
            const LayerConfig& layer = config.layers[i];
            double wiggle = noise_surface.value({ (double)x * layer.noise_scale, (double)seed + i * 100.0 }) * layer.variation;
            current_y += layer.thickness + (int)wiggle;
            layer_limits[x][i] = current_y;
        }
    }

    // Place blocks using precomputed biome and layer limits
    #pragma omp parallel for // Parallelise this loop because each loop inside is independant, it's writing to a new value each thread
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int warped_x = warp_map[x][y];
            BiomeType block_type = biome_map[warped_x];
            blocks[x][y] = get_block_at(warped_x, y, block_type, layer_limits[x]);
        }
    }
}

// Get block at a point in a specific biome and layer
Block World::get_block_at(int x, int y, BiomeType block_type, const std::vector<int> &layer_limit)
{
    if (y < surface_map[x]) return Block(Air);

    size_t layer_index = layer_limit.size();
    for (size_t i = 0; i < layer_limit.size(); i++) {
        if (y < layer_limit[i]) {
            layer_index = i;
            break;
        }
    }

    return Block(
        get_block_type_for_biome(block_type, x, y),
        Solid,
        get_wall_type_for_biome(block_type, layer_index)
    );
}

// Helper Functions
int World::get_layer_at(int x, int y)
{
    int current_y = surface_map[x];
    for (size_t i = 0; i < config.layers.size(); i++) {
        const LayerConfig& layer = config.layers[i];
        double wiggle = noise_surface.value({ (double)x * layer.noise_scale, (double)seed + (i * 100.0) }) * layer.variation;
        current_y += (layer.thickness + (int) wiggle);
        if (y < current_y) return (int)i;
    }
    return (int) config.layers.size();
}

BlockType World::get_block_type_for_biome(BiomeType type, int x, int y)
{
    int layer_index = get_layer_at(x, y);
    Biome& biome = get_biome_data().at(type);

    switch (layer_index) {
        case 0: return biome.surface_block;
        case 1: return biome.subsurface_block;
        case 2: return biome.underground_block;
        case 3: return biome.cavern_block;
        default: return biome.underworld_block;
    }
}

WallType World::get_wall_type_for_biome(BiomeType type, int layer_index)
{
    Biome& biome = get_biome_data().at(type);

    switch (layer_index) {
        case 0: return AirWall;
        case 1: return biome.underground_wall;
        case 2: return biome.underground_wall;
        case 3: return biome.cavern_wall;
        default: return biome.underworld_wall;
    }
}

int World::get_surface_height(int x)
{
    double amp = height * config.terrain.amplitude_ratio;
    double big = noise_surface.value({ (double) x * config.terrain.big_scale, (double) seed });
    double small = noise_surface.value({ (double) x * config.terrain.small_scale, (double) seed * 2.0 });

    return (int)(surface_base + (big * amp) + (small * amp * config.terrain.small_weight));
}

void World::draw()
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Block &block = blocks[x][y];
            int bx = x * BLOCK_SIZE * zoom;
            int by = y * BLOCK_SIZE * zoom;
            int size = BLOCK_SIZE * zoom;

            if (block.type != Air) { fill_rectangle(block_colors[block.type], bx, by, size, size);}
            else if (block.wall != AirWall) { fill_rectangle(wall_colors[block.wall], bx, by, size, size); }
            else { fill_rectangle(block_colors[Air], bx, by, size, size); }
        }
    }
}