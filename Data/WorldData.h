#pragma once
#include "splashkit.h"
#include "splashkit-arrays.h"

#include "BlockData.h"
#include "NoiseData.h"

#include <vector>

class World
{
    public:
        int width, height;
        unsigned int seed;
        int zoom;

        std::vector<std::vector<Block>> blocks;
    
    private:
        Noise noise;

    public:
        World(int width_value, int height_value, unsigned int seed_value, int zoom_value) : width(width_value), height(height_value), seed(seed_value), zoom(zoom_value), noise(seed_value) 
        { 
            blocks.resize(width);
            for (int x = 0; x < width; x++)
            {
                blocks[x].resize(height, Block(Air, Solid));
            }
        }

        void generate()
        {
            for(int x = 0; x < width; x++)
            {
                for(int y = 0; y < height; y++)
                {
                    double val = noise.value({(double) x, (double) y});
                    if(val < 0.3) blocks[x][y] = Block(Air, Solid);
                    else if(val < 0.5) blocks[x][y] = Block(Dirt, Solid);
                    else if(val < 0.8) blocks[x][y] = Block(Grass, Solid);
                    else blocks[x][y] = Block(Stone, Solid);
                }
            }
        }

        void draw()
        {
            for(int x = 0; x < width; x++)
            {
                for(int y = 0; y < height; y++)
                {
                    Block &b = blocks[x][y];
                    fill_rectangle(block_colors[b.type], x * zoom * BLOCK_SIZE, y * zoom * BLOCK_SIZE, BLOCK_SIZE * zoom, BLOCK_SIZE * zoom);
                }
            }
        }
};
