#include "World.h"
#include "../Block/Block.h"
#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"

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

World::World(int w, int h, unsigned int s, int z, const WorldConfig& cfg) : width(w), height(h), seed(s), zoom(z), config(cfg),
      noise_surface(s + cfg.noise.surface_offset),
      noise_underground(s + cfg.noise.underground_offset)
{
    surface_base = height * config.terrain.base_height_ratio;
    blocks.assign(width, std::vector<Block>(height, Block(Air)));
}

// Surface Height

int World::get_surface_height(int x)
{
    double amplitude = height * config.terrain.amplitude_ratio;

    double big = noise_surface.value({x * config.terrain.big_scale, 0.0}) * amplitude;

    double small = noise_surface.value({x * config.terrain.small_scale, 0.0}) *
                   (amplitude * config.terrain.small_weight);

    return int(surface_base + big + small);
}

// Generate Terrain

void World::generate()
{
    biome_map = generate_biome_map(width, seed);
    std::vector<int> surface_map(width);

    for (int x = 0; x < width; x++)
    {
        surface_map[x] = get_surface_height(x);
    }

    for (int x = 0; x < width; x++)
    {
        int surface = surface_map[x];
        Biome &biome = biome_data[biome_map[x]];

        for (int y = 0; y < height; y++)
        {
            if (y < surface)
            {
                blocks[x][y] = Block(Air);
                continue;
            }

            int depth = y - surface;

            if (depth == 0)
            {
                blocks[x][y] = Block(biome.surface_block, Solid, AirWall);
                continue;
            }

            bool placed = false;
            int layer_top = surface;
            
            for (size_t i = 0; i < config.layers.size(); i++)
            {
                const LayerConfig &layer = config.layers[i];
                double n = noise_underground.value({x * layer.noise_scale, layer_top * layer.noise_scale});
                int layer_bottom = layer_top + layer.thickness + int(n * layer.variation);

                if (y >= layer_top && y < layer_bottom)
                {
                    BlockType block_type;
                    WallType wall_type;

                    switch (i)
                    {
                        case 0: block_type = biome.surface_block; wall_type = AirWall; break;
                        case 1: block_type = biome.subsurface_block; wall_type = biome.underground_wall; break;
                        case 2: block_type = biome.underground_block; wall_type = biome.underground_wall; break;
                        case 3: block_type = biome.cavern_block; wall_type = biome.cavern_wall; break;
                        default: block_type = biome.underworld_block; wall_type = biome.underworld_wall; break;
                    }

                    blocks[x][y] = Block(block_type, Solid, wall_type);
                    placed = true;
                    break;
                }

                layer_top = layer_bottom;
            }
            if (!placed)
            {
                const LayerConfig &last = config.layers.back();
                blocks[x][y] = Block(last.block, Solid, last.wall);
            }
        }
    }
}

// Draw World

void World::draw()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            const Block &block = blocks[x][y];

            int bx = x * BLOCK_SIZE * zoom;
            int by = y * BLOCK_SIZE * zoom;
            int size = BLOCK_SIZE * zoom;

            if (block.type != Air)
                fill_rectangle(block_colors[block.type], bx, by, size, size);
            else if (block.wall != AirWall)
                fill_rectangle(wall_colors[block.wall], bx, by, size, size);
            else
                fill_rectangle(block_colors[Air], bx, by, size, size);
        }
    }
}