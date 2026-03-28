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
        
        int surface_base, terrain_amplitude;

        World(int w, int h, unsigned int s, int z) : width(w), height(h), seed(s), zoom(z), noise(s)
        {
            surface_base = height * 0.2;
            terrain_amplitude = height * 0.1;

            blocks.assign(width, std::vector<Block>(height, Block(Air)));
            layer_config = {8, 40, 50, 20};
        }
        
        int get_surface_height(int x) {
            double big_hills_noise = (noise.value({x * 0.5, 0.0}) + 1.0) * terrain_amplitude;
            double small_bumps_noise = (noise.value({x * 0.1, 0.0}) + 1.0) * (terrain_amplitude / 2);
            return int(surface_base + big_hills_noise + small_bumps_noise);
        }

        std::vector<int> get_layer_offsets(int x) {
            return {
                (int)(noise.value({x * 0.03, 100.0}) * 10),
                (int)(noise.value({x * 0.02, 200.0}) * 20),
                (int)(noise.value({x * 0.01, 300.0}) * 30),
                (int)(noise.value({x * 0.005, 400.0}) * 40)
            };
        }

        WorldLayer get_layer(int y, int surface_height, const std::vector<int>& offsets)
        {
            int current_boundary = surface_height;
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
                current_boundary += layers[i].depth + offsets[i]; 
                if (y < current_boundary) return layers[i].type;
            }

            return UNDERWORLD;
        }

        void generate()
        {
            for (int x = 0; x < width; x++)
            {
                int surface_height = get_surface_height(x);
                std::vector<int> offsets = get_layer_offsets(x);

                for (int y = 0; y < height; y++)
                {
                    if (y < surface_height) {
                        blocks[x][y] = Block(Air);
                    } else {
                        int depth = y - surface_height;
                        switch (get_layer(y, surface_height, offsets))
                        {
                            case SURFACE: blocks[x][y] = (depth < 3) ? Block(Grass) : Block(Dirt); break;
                            case UNDERGROUND: blocks[x][y] = (depth < 10) ? Block(Dirt) : Block(Stone); break;
                            case CAVERN: blocks[x][y] = Block(Deepstone); break;
                            case UNDERWORLD: blocks[x][y] = Block(Hellstone); break;
                            default: blocks[x][y] = Block(Air); break;
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
        Noise noise;
};