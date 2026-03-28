#pragma once
#include "splashkit.h"
#include "splashkit-arrays.h"
#include "WorldGenerationHeaders.h"
#include <vector>

enum WorldLayer { SPACE, SURFACE, UNDERGROUND, CAVERN, UNDERWORLD };

struct LayerConfigData { 
    int surface_depth, underground_depth, cavern_depth, underworld_depth;
};

class World
{
    public:
        int width, height;
        unsigned int seed;
        int zoom;
        std::vector<std::vector<Block>> blocks;
        LayerConfigData layer_config;

        World(int w, int h, unsigned int s, int z) : width(w), height(h), seed(s), zoom(z), noise(s)
        {
            surface_base = height * 0.2;
            terrain_amplitude = height * 0.1;

            blocks.assign(width, std::vector<Block>(height, Block(Air, Solid)));
            layer_config = {5, 40, 50, 20};
        }

        WorldLayer get_layer(int y, int surface_height)
        {
            int bottom = surface_height;
            struct Layer { WorldLayer type; int depth; };
            Layer layers[] = {
                {SURFACE, layer_config.surface_depth},
                {UNDERGROUND, layer_config.underground_depth},
                {CAVERN, layer_config.cavern_depth},
                {UNDERWORLD, layer_config.underworld_depth}
            };

            if (y < surface_height) return SPACE;

            for (int i = 0; i < 4; i++)
            {
                bottom += layers[i].depth;
                if (y < bottom) return layers[i].type;
            }

            return UNDERWORLD;
        }

        void generate()
        {
            for (int x = 0; x < width; x++)
            {
                double big_hills_noise = (noise.value({x * 0.5, 0.0}) + 1.0) * terrain_amplitude;
                double small_bumps_noise = (noise.value({x * 0.1, 0.0}) + 1.0) * (terrain_amplitude / 2);
                
                int surface_height = int(surface_base + big_hills_noise + small_bumps_noise);

                for (int y = 0; y < height; y++)
                {
                    if (y < surface_height)
                        blocks[x][y] = Block(Air, Solid);
                    else
                    {
                        int depth = y - surface_height;
                        switch (get_layer(y, surface_height))
                        {
                            case SURFACE: blocks[x][y] = (depth == 0) ? Block(Grass, Solid) : Block(Dirt, Solid); break;
                            case UNDERGROUND: blocks[x][y] = (depth < 5) ? Block(Dirt, Solid) : Block(Stone, Solid); break;
                            case CAVERN: case UNDERWORLD: blocks[x][y] = Block(Stone, Solid); break;
                            default: blocks[x][y] = Block(Air, Solid); break;
                        }
                    }
                }
            }
        }

        void draw()
        {
            for (int x = 0; x < width; x++){
                for (int y = 0; y < height; y++){
                    fill_rectangle(block_colors[blocks[x][y].type], x * BLOCK_SIZE * zoom, y * BLOCK_SIZE * zoom, BLOCK_SIZE * zoom, BLOCK_SIZE * zoom);
                }
            }
        }

    private:
        int surface_base, terrain_amplitude;
        Noise noise;
};