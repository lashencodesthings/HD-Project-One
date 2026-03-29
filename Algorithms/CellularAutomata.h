#pragma once
#include "splashkit.h"
#include "../Data/WorldGenerationHeaders.h"
#include <random>

class CellularAutomata {
public:
    CellularAutomata(World &target_world) : world(target_world), rng(target_world.seed) {}

    void run() {
        world.blocks = seed_initial_noise_pockets();
        apply();
    }

private:
    World &world;
    std::mt19937 rng;

    const int WALL_THRESHOLD = 4;
    const int NOISE_DENSITY = 35;
    const int ITERATIONS = 12;
    
    const int OFFSET = 5;

    int random_percent() {
        std::uniform_int_distribution<int> dist(0, 99);
        return dist(rng);
    }

    std::vector<std::vector<Block>> seed_initial_noise_pockets() {
        std::vector<std::vector<Block>> noise_grid = world.blocks;
        for (int x = 0; x < world.width; x++) {
            for (int y = 0; y < world.height; y++) {
                int random = random_percent();
                if (y < world.get_surface_height(x) + OFFSET) continue;

                if(random < NOISE_DENSITY) { 
                    noise_grid[x][y] = Block(BlockType::Air, Solid, noise_grid[x][y].wall); 
                }
                else
                {
                    noise_grid[x][y] = Block(noise_grid[x][y].wall);
                }
            }
        }
        return noise_grid;
    }

    void apply()
    {
        int width = world.width;
        int height = world.height;

        std::vector<bool> current_bits(width * height);
        std::vector<bool> iteration_bits(width * height);
        std::vector<int> surface_cache(width);

        for (int x = 0; x < width; x++) {
            surface_cache[x] = world.get_surface_height(x) + OFFSET;
            for (int y = 0; y < height; y++) {
                current_bits[x + y * width] = (world.blocks[x][y].type != BlockType::Air);
            }
        }

        for (int iteration = 0; iteration < ITERATIONS; iteration++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int neighbors = 0;
                    for (int ny = y - 1; ny <= y + 1; ny++) {
                        for (int nx = x - 1; nx <= x + 1; nx++) {
                            if (nx == x && ny == y) continue;
                            if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                                neighbors++;
                            } else if (current_bits[nx + ny * width]) {
                                neighbors++;
                            }
                        }
                    }
                    iteration_bits[x + y * width] = (neighbors > WALL_THRESHOLD);
                }
            }
            current_bits.swap(iteration_bits); // Swap pointers rather than reassigning to use less memory
        }

        for (int x = 0; x < width; x++) {
            for (int y = surface_cache[x]; y < height; y++) {
                if (current_bits[x + y * width])
                    world.blocks[x][y] = Block(world.blocks[x][y].wall);
                else
                    world.blocks[x][y] = Block(BlockType::Air, Solid, world.blocks[x][y].wall);
            }
        }
    }
};