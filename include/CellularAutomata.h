#pragma once
#include "splashkit.h"
#include "../Data/WorldGenerationHeaders.h"
#include <random>
#include <vector>

class CellularAutomata {
public:
    CellularAutomata(World& world_data) : world(world_data), rng(world_data.seed) {}

    void run() {
        seed_noise();
        apply();
    }

private:
    World& world;
    std::mt19937 rng;

    const int WALL_THRESHOLD = 4;
    const int NOISE_DENSITY = 35;
    const int ITERATIONS = 12;
    const int OFFSET = 5;

    std::uniform_int_distribution<int> percent_dist{0, 99};

    inline bool add_air_hole() { return percent_dist(rng) < NOISE_DENSITY; }

    void seed_noise() {
        for (int x = 0; x < world.width; ++x) {
            int surface_height = world.get_surface_height(x) + OFFSET;

            for (int y = surface_height; y < world.height; ++y) {
                if (add_air_hole())
                    world.blocks[x][y] = Block(BlockType::Air, Solid, world.blocks[x][y].wall);
                else
                    world.blocks[x][y] = Block(world.blocks[x][y].wall);
            }
        }
    }

    void apply() {
        const int width = world.width;
        const int height = world.height;

        std::vector<uint8_t> current(width * height);
        std::vector<uint8_t> next(width * height);
        std::vector<int> surface_height(width);

        for (int x = 0; x < width; ++x) {
            surface_height[x] = world.get_surface_height(x) + OFFSET;
            for (int y = 0; y < height; ++y) { current[x + y * width] = (world.blocks[x][y].type != BlockType::Air); }
        }

        for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
            next = current;
            for (int y = 1; y < height - 1; ++y) {
                int row = y * width;

                for (int x = 1; x < width - 1; ++x) {
                    int block_index = x + row;
                    int neighbours =
                        current[block_index - width - 1] + current[block_index - width] + current[block_index - width + 1] +
                        current[block_index - 1] + current[block_index + 1] +
                        current[block_index + width - 1] + current[block_index + width] + current[block_index + width + 1];

                    next[block_index] = (neighbours > WALL_THRESHOLD);
                }
            }
            std::swap(current, next); 
        }

        for (int x = 0; x < width; ++x) {
            for (int y = surface_height[x]; y < height; ++y) {
                int block_index = x + y * width;
                world.blocks[x][y] = current[block_index] ? Block(world.blocks[x][y].wall) : Block(BlockType::Air, Solid, world.blocks[x][y].wall);
            }
        }
    }
};