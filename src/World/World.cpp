#include "World.h"
#include "../Block/Block.h"
#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"

const LayerConfig WorldDefaults::layers{20, 50, 200, 20};
const TerrainConfig WorldDefaults::terrain{0.5, 0.05, 0.25, 0.1, 1};
const NoiseConfig WorldDefaults::noise{0, 100, 200};

const int WorldDefaults::surface_variation = 5;
const int WorldDefaults::underground_variation = 10;
const int WorldDefaults::cavern_variation = 15;

World::World(int w, int h, unsigned int s, int z)
    : width(w), height(h), seed(s), zoom(z),
      noise_surface(s + WorldDefaults::noise.surface_offset),
      noise_underground(s + WorldDefaults::noise.underground_offset),
      noise_cavern(s + WorldDefaults::noise.cavern_offset),
      layer_config(WorldDefaults::layers),
      terrain_config(WorldDefaults::terrain)
{
    surface_base = height * 0.2;
    blocks.assign(width, std::vector<Block>(height, Block(Air)));
}

int World::get_surface_height(int x)
{
    double amplitude = height * terrain_config.amplitude_ratio;

    double big = noise_surface.value({x * terrain_config.big_scale, 0.0}) * amplitude;
    double small = noise_surface.value({x * terrain_config.small_scale, 0.0}) * (amplitude * terrain_config.small_weight);

    return int(surface_base + big + small);
}

WorldLayer World::get_layer(double x, double y, int surface)
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

void World::smooth(std::vector<int> &map)
{
    int r = terrain_config.smoothing_radius;
    int window = 2 * r + 1;

    std::vector<int> result = map;

    int sum = 0;

    for (int i = 0; i < window; i++)
        sum += map[i];

    for (int x = r; x < width - r; x++)
    {
        result[x] = sum / window;

        if (x + r + 1 < width)
            sum += map[x + r + 1];

        if (x - r >= 0)
            sum -= map[x - r];
    }

    map = result;
}

void World::generate()
{
    biome_map = generate_biome_map(width, seed);
    std::vector<int> surface_map(width);

    for (int x = 0; x < width; x++)
    {
        surface_map[x] = get_surface_height(x);
    }

    smooth(surface_map);

    for (int x = 0; x < width; x++)
    {
        int surface_height = surface_map[x];
        Biome &biome = biome_data[biome_map[x]];

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
                blocks[x][y] = Block(biome.surface_block, Solid, AirWall);
                continue;
            }

            WorldLayer layer = get_layer(x, y, surface_height);

            switch (layer)
            {
                case WorldLayer::Surface:
                    blocks[x][y] = Block(biome.subsurface_block, Solid, biome.underground_wall);
                    break;

                case WorldLayer::Underground:
                    blocks[x][y] = Block(biome.underground_block, Solid, biome.underground_wall);
                    break;

                case WorldLayer::Cavern:
                    blocks[x][y] = Block(biome.cavern_block, Solid, biome.cavern_wall);
                    break;

                case WorldLayer::Underworld:
                    blocks[x][y] = Block(biome.underworld_block, Solid, biome.underworld_wall);
                    break;

                default:
                    blocks[x][y] = Block(Air);
                    break;
            }
        }
    }
}

void World::draw()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            const Block &block = blocks[x][y];

            int block_x = x * BLOCK_SIZE * zoom;
            int block_y = y * BLOCK_SIZE * zoom;
            int size = BLOCK_SIZE * zoom;

            if (block.type != Air)
                fill_rectangle(block_colors[block.type], block_x, block_y, size, size);
            else if (block.wall != AirWall)
                fill_rectangle(wall_colors[block.wall], block_x, block_y, size, size);
            else
                fill_rectangle(block_colors[Air], block_x, block_y, size, size);
        }
    }
}