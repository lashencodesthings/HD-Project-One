#pragma once
#include "splashkit.h"

#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"

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
    extern const LayerConfig layers;
    extern const TerrainConfig terrain;
    extern const NoiseConfig noise;

    extern const int surface_variation;
    extern const int underground_variation;
    extern const int cavern_variation;
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

        std::vector<BiomeType> biome_map;

        const int BLOCK_SIZE = 2;

        World(int w, int h, unsigned int s, int z);

        int get_surface_height(int x);
        WorldLayer get_layer(double x, double y, int surface);
        void smooth(std::vector<int> &map);
        void generate();
        void draw();
};