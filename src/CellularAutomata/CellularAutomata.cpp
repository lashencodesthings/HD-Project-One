#include "CellularAutomata.h"
#include "splashkit.h"
#include "../World/World.h"

CellularAutomata::CellularAutomata(World& world_data) : world(world_data), rng(world_data.seed) {}

void CellularAutomata::run() {
    seed_noise();
    apply();
}

bool CellularAutomata::add_air_hole() { 
    return percent_dist(rng) < NOISE_DENSITY; 
}

void CellularAutomata::seed_noise() {
    for (int x = 0; x < world.width; ++x) {
        int surface_height = world.surface_map[x];
        for (int y = surface_height; y < world.height; ++y) {
            if (add_air_hole()) {
                Block &old = world.blocks[x][y];
                world.blocks[x][y] = Block(BlockType::Air, Solid, old.wall);
            }
        }
    }
}

void CellularAutomata::apply() {
    const int width = world.width;
    const int height = world.height;

    std::vector<uint8_t> current(width * height);
    std::vector<uint8_t> next(width * height);

    for (int x = 0; x < width; ++x) {
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
        for (int y = world.surface_map[x]; y < height; ++y) {
            int block_index = x + y * width;
            if (current[block_index]) {
                world.blocks[x][y] = world.get_block_at(x, y);
            } else {
                world.blocks[x][y] = Block(BlockType::Air, Solid, world.blocks[x][y].wall);
            }
        }
    }
}