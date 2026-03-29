#pragma once
#include "splashkit.h"
#include "splashkit-arrays.h"
#include "WorldGenerationHeaders.h"
#include <vector>
#include <array>

enum class WorldLayer { Space, Surface, Underground, Cavern, Underworld };

struct LayerConfig
{
    int surface;
    int underground;
    int cavern;
    int underworld;
};

struct TerrainConfig
{
    double big_scale;
    double small_scale;
    double small_weight;
    double amplitude_ratio;
    int smoothing_radius;
};

struct NoiseConfig
{
    int surface_offset;
    int underground_offset;
    int cavern_offset;
};

namespace WorldDefaults
{
    const LayerConfig layers{20, 50, 200, 20};
    const TerrainConfig terrain{0.5, 0.05, 0.25, 0.1, 1};
    const NoiseConfig noise{0, 100, 200};

    const int surface_variation = 5;
    const int underground_variation = 10;
    const int cavern_variation = 15;

    const int dirt_depth = 5;
    const double stone_threshold = 0.2;
}

class World
{
public:
    int width, height;
    unsigned int seed;
    int zoom;

    std::vector<std::vector<Block>> blocks;
    
    Noise noise_surface;
    Noise noise_underground;
    Noise noise_cavern;

    LayerConfig layer_config;
    TerrainConfig terrain_config;

    int surface_base;

    World(int w, int h, unsigned int s, int z) : width(w), height(h), seed(s), zoom(z), 
          noise_surface(s + WorldDefaults::noise.surface_offset), noise_underground(s + WorldDefaults::noise.underground_offset),
          noise_cavern(s + WorldDefaults::noise.cavern_offset), layer_config(WorldDefaults::layers), terrain_config(WorldDefaults::terrain)
    {
        surface_base = 0;
        blocks.assign(width, std::vector<Block>(height, Block(Air)));
    }

    int get_surface_height(int x)
    {
        double amplitude = height * terrain_config.amplitude_ratio;

        double big = (noise_surface.value({x * terrain_config.big_scale, 0.0}) + 1.0) * amplitude;
        double small = (noise_surface.value({x * terrain_config.small_scale, 0.0}) + 1.0) * (amplitude * terrain_config.small_weight);

        return int(surface_base + big + small);
    }

    WorldLayer get_layer(double x, double y, int surface)
    {
        double s_noise = noise_surface.value({x, y});
        double u_noise = noise_underground.value({x, y});
        double c_noise = noise_cavern.value({x, y});

        int surface_end = surface + layer_config.surface + int(s_noise * WorldDefaults::surface_variation);
        int underground_end = surface_end + layer_config.underground + int(u_noise * WorldDefaults::underground_variation);
        int cavern_end = underground_end + layer_config.cavern + int(c_noise * WorldDefaults::cavern_variation);

        if (y < surface) return WorldLayer::Space;
        if (y < surface_end) return WorldLayer::Surface;
        if (y < underground_end) return WorldLayer::Underground;
        if (y < cavern_end) return WorldLayer::Cavern;

        return WorldLayer::Underworld;
    }

    void smooth(std::vector<int> &map)
    {
        std::vector<int> copy = map;

        for (int x = terrain_config.smoothing_radius; x < width - terrain_config.smoothing_radius; x++)
        {
            int sum = 0;
            int count = 0;

            for (int i = -terrain_config.smoothing_radius; i <= terrain_config.smoothing_radius; i++)
            {
                sum += copy[x + i];
                count++;
            }

            map[x] = sum / count;
        }
    }

    void generate()
    {
        std::vector<int> surface_map(width);

        for (int x = 0; x < width; x++)
            surface_map[x] = get_surface_height(x);

        smooth(surface_map);

        for (int x = 0; x < width; x++)
        {
            int surface_height = surface_map[x];

            for (int y = 0; y < height; y++)
            {
                if (y < surface_height)
                {
                    blocks[x][y] = Block(Air);
                    continue;
                }

                int depth = y - surface_height;

                if (depth == 0)
                {
                    blocks[x][y] = Block(Grass, Solid, AirWall);
                    continue;
                }

                if (depth < WorldDefaults::dirt_depth)
                {
                    blocks[x][y] = Block(Dirt, Solid, UndergroundWall);
                    continue;
                }

                WorldLayer layer = get_layer(x, y, surface_height);

                switch (layer)
                {
                    case WorldLayer::Surface:
                    case WorldLayer::Underground:
                    {
                        double n = noise_underground.value({x * 0.1, y * 0.1});

                        if (n > WorldDefaults::stone_threshold)
                            blocks[x][y] = Block(Stone, Solid, UndergroundWall);
                        else
                            blocks[x][y] = Block(Dirt, Solid, UndergroundWall);

                        break;
                    }

                    case WorldLayer::Cavern:
                        blocks[x][y] = Block(Cavernstone, Solid, CavernWall);
                        break;

                    case WorldLayer::Underworld:
                        blocks[x][y] = Block(Hellstone, Solid, HellWall);
                        break;

                    default:
                        blocks[x][y] = Block(Air);
                        break;
                }
            }
        }
    }

    void draw()
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                const Block &block = blocks[x][y];

                int block_x = x * BLOCK_SIZE * zoom;
                int block_y = y * BLOCK_SIZE * zoom;
                int size = BLOCK_SIZE * zoom;

                if (block.type != Air) { fill_rectangle(block_colors[block.type], block_x, block_y, size, size); }
                else if (block.wall != AirWall) { fill_rectangle(wall_colors[block.wall], block_x, block_y, size, size); }
                else { fill_rectangle(block_colors[Air], block_x, block_y, size, size); }
            }
        }
    }
};