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

// Generate Main Procedure
void World::generate()
{
    biome_map = generate_biome_map(width, seed);
    surface_map.resize(width);
    
    // Pass 1 Initial Placement
    placement_pass();
}

void World::placement_pass()
{
    for (int x = 0; x < width; x++) {
        surface_map[x] = get_surface_height(x);
        for (int y = 0; y < height; y++) { blocks[x][y] = get_block_at(x, y);  }
    }
}

// Helper Functions
Block World::get_block_at(int x, int y)
{
    int surface = surface_map[x];
    if (y < surface) return Block(Air);

    int layer_index = get_layer_at(x, y);
    
    // I don't want straight lines between biomes, so I warp the whole map
    double warp1 = noise_biome.value({ x * 0.02, y * 0.02 }) * 80.0;
    double warp2 = noise_biome.value({ x * 0.05 + 100.0, y * 0.05 + 100.0 }) * 40.0;

    double warp = warp1 + warp2;

    int sample_x = x + (int) warp;
    sample_x = std::max(0, std::min(width - 1, sample_x));

    BiomeType b_type = biome_map[sample_x];
        
    return Block(
        get_block_type_for_biome(b_type, x, y), 
        Solid, 
        get_wall_type_for_biome(b_type, layer_index)
    );
}

int World::get_layer_at(int x, int y)
{
    int current_y = surface_map[x];
    for (size_t i = 0; i < config.layers.size(); i++) {
        const LayerConfig& layer = config.layers[i];
        double wiggle = noise_surface.value({ (double)x * layer.noise_scale, (double)seed + (i * 100.0) }) * layer.variation;
        current_y += (layer.thickness + (int)wiggle);
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

// Calculation Functions
BiomeType World::get_nearest_neighbour(int x, int& out_dist) {
    int search_radius = 50;

    for (int r = 1; r < search_radius; r++) {
        for (int dx = -r; dx <= r; dx++) {
            int nx = x + dx;
            if (nx < 0 || nx >= width) continue;

            if (biome_map[nx] != biome_map[x]) {
                out_dist = r;
                return biome_map[nx];
            }
        }
    }

    out_dist = search_radius;
    return biome_map[x];
}

int World::get_surface_height(int x)
{
    double amp = height * config.terrain.amplitude_ratio;
    double big = noise_surface.value({ (double) x * config.terrain.big_scale, (double) seed });
    double small = noise_surface.value({ (double) x * config.terrain.small_scale, (double) seed * 2.0 });

    double height_value = surface_base + (big * amp) + (small * amp * config.terrain.small_weight);
    return (int) height_value;
}

void World::draw()
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Block &block = blocks[x][y];
            int bx = x * BLOCK_SIZE * zoom;
            int by = y * BLOCK_SIZE * zoom;
            int size = BLOCK_SIZE * zoom;

            if (block.type != Air) {
                fill_rectangle(block_colors[block.type], bx, by, size, size);
            } else if (block.wall != AirWall) {
                fill_rectangle(wall_colors[block.wall], bx, by, size, size);
            } else {
                fill_rectangle(block_colors[Air], bx, by, size, size);
            }
        }
    }
}