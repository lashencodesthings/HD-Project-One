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

        World(int w, int h, unsigned int s, int z) : width(w), height(h), seed(s), zoom(z), noise_surface(s), noise_underground(s + 100), noise_cavern(s + 200)
        {
            surface_base = 0;
            terrain_amplitude = height * 0.1;

            blocks.assign(width, std::vector<Block>(height, Block(Air)));
            layer_config = {20, 50, 200, 20};
        }
        
        int get_surface_height(int x) {
            double big_hills_noise = (noise_surface.value({x * 0.5, 0.0}) + 1.0) * terrain_amplitude;
            double small_bumps_noise = (noise_surface.value({x * 0.05, 0.0}) + 1.0) * (terrain_amplitude / 4);
            return int(surface_base + big_hills_noise + small_bumps_noise);
        }

        WorldLayer get_layer(double x, double y, int surface)
        {
            double surface_noise = noise_surface.value({x, y});
            double underground_noise = noise_underground.value({x, y});
            double cavern_noise = noise_cavern.value({x, y});

            int surface_end = surface + layer_config.surface_depth + int(surface_noise * 5);
            int underground_end = surface_end + layer_config.underground_depth + int(underground_noise * 10);
            int cavern_end = underground_end + layer_config.cavern_depth + int(cavern_noise * 15);

            if (y < surface) return SPACE;
            if (y < surface_end) return SURFACE;
            if (y < underground_end) return UNDERGROUND;
            if (y < cavern_end) return CAVERN;

            return UNDERWORLD;
        }

        void generate()
        {
            std::vector<int> surface_map(width);
            for (int x = 0; x < width; x++) {
                surface_map[x] = get_surface_height(x);
            }

            for (int x = 1; x < width - 1; x++) {
                surface_map[x] = (surface_map[x-1] + surface_map[x] + surface_map[x+1]) / 3;
            }

            for (int x = 0; x < width; x++)
            {
                int surface_height = surface_map[x]; 
                
                for (int y = 0; y < height; y++)
                {
                    if (y < surface_height) {
                        blocks[x][y] = Block(Air);
                    } else {
                        int depth = y - surface_height;

                        if (depth == 0) {
                            blocks[x][y] = Block(Grass, Solid, AirWall);
                        } 
                        else if (depth < 5){
                            blocks[x][y] = Block(Dirt, Solid, UndergroundWall);
                        }
                        else {
                            WorldLayer current_layer = get_layer(x, y, surface_height);

                            switch(current_layer) 
                            {
                                case SURFACE:
                                case UNDERGROUND:
                                {
                                    double stone_noise = noise_underground.value({x * 0.1, y * 0.1});
                                    if (stone_noise > 0.2)
                                        blocks[x][y] = Block(Stone, Solid, UndergroundWall);
                                    else
                                        blocks[x][y] = Block(Dirt, Solid, UndergroundWall);
                                    break;
                                }
                                case CAVERN: 
                                    blocks[x][y] = Block(Cavernstone, Solid, CavernWall); 
                                    break;
                                case UNDERWORLD: 
                                    blocks[x][y] = Block(Hellstone, Solid, HellWall); 
                                    break;
                                default: 
                                    blocks[x][y] = Block(Air); 
                                    break;
                            }
                        }
                    }
                }
            }
        }

        void draw()
        {
            for (int x = 0; x < width; x++){
                for (int y = 0; y < height; y++){
                    
                    if (blocks[x][y].type != Air) {
                        fill_rectangle(
                            block_colors[blocks[x][y].type],
                            x * BLOCK_SIZE * zoom,
                            y * BLOCK_SIZE * zoom,
                            BLOCK_SIZE * zoom,
                            BLOCK_SIZE * zoom
                        );
                    }
                    else
                    {
                        if (blocks[x][y].type == Air && blocks[x][y].back_wall != AirWall) {
                            fill_rectangle(
                                wall_colors[blocks[x][y].back_wall],
                                x * BLOCK_SIZE * zoom,
                                y * BLOCK_SIZE * zoom,
                                BLOCK_SIZE * zoom,
                                BLOCK_SIZE * zoom
                            );
                        }
                        else{
                            fill_rectangle(
                                block_colors[Air],
                                x * BLOCK_SIZE * zoom,
                                y * BLOCK_SIZE * zoom,
                                BLOCK_SIZE * zoom,
                                BLOCK_SIZE * zoom
                            );
                        }
                    }
                }
            }
        }
    
    private:
        Noise noise_surface;
        Noise noise_underground;
        Noise noise_cavern;
};