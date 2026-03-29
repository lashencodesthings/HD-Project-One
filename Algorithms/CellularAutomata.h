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
    const int NOISE_DENSITY = 40;
    const int ITERATIONS = 10;

    int random_percent() {
        std::uniform_int_distribution<int> dist(0, 99);
        return dist(rng);
    }

    std::vector<std::vector<Block>> seed_initial_noise_pockets() {
        std::vector<std::vector<Block>> noise_grid = world.blocks;
        for (int x = 0; x < world.width; x++) {
            for (int y = 0; y < world.height; y++) {
                int random = random_percent();
                if (y < world.get_surface_height(x) + 8) continue;

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
        for(int iteration = 0; iteration < ITERATIONS; iteration++)
        {
            std::vector<std::vector<Block>> temporary_grid = world.blocks; 
            for(int i = 0; i < world.width; i++)
            {
                for(int j = 0; j < world.height; j++)
                {
                    if (j < world.get_surface_height(i) + 8) continue;

                    int neighbour_wall_count = 0;
                    for(int x = i - 1; x <= i + 1; x++)
                    {
                        for(int y = j - 1; y <= j + 1; y++)
                        {
                            if(x == i && y == j) continue;

                            if(x >= 0 && x < world.width && y >= 0 && y < world.height)
                            {
                                if(world.blocks[x][y].type != BlockType::Air)
                                {
                                    neighbour_wall_count++;
                                }
                            }
                            else
                            {
                                neighbour_wall_count++;
                            }
                        }
                    }
                    if(neighbour_wall_count <= WALL_THRESHOLD)
                    {
                        temporary_grid[i][j] = Block(BlockType::Air, Solid, world.blocks[i][j].wall); 
                    }
                    else
                    {
                        temporary_grid[i][j] = Block(world.blocks[i][j].wall); 
                    }
                }
            }
            world.blocks = temporary_grid;
        }
    }
};