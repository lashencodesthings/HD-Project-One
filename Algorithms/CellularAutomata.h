#pragma once
#include "splashkit.h"
#include "../Data/WorldGenerationHeaders.h"
#include <cstdlib>

class CellularAutomata {
    public:
        CellularAutomata(World &target_world) : world(target_world) {}

        void run(int range_top_y, int range_bottom_y, int evolution_iterations, int initial_fill_probability) {
            srand(world.seed); 

            seed_initial_noise_pockets(range_top_y, range_bottom_y, initial_fill_probability);

            for (int i = 0; i < evolution_iterations; i++) {
                smooth_cave_walls(range_top_y, range_bottom_y, 5, 20);
            }

            smooth_cave_walls(range_top_y, range_bottom_y, 5, 5);
            fill_isolated_air_pockets(range_top_y, range_bottom_y, 7, 5);
            smooth_cave_walls(range_top_y, range_bottom_y, 6, 5);
        }

    private:
        World &world;

        void seed_initial_noise_pockets(int range_top_y, int range_bottom_y, int fill_probability) {
            for (int x_coordinate = 0; x_coordinate < world.width; x_coordinate++) {
                int surface_height = world.get_surface_height(x_coordinate);
                for (int y_coordinate = range_top_y; y_coordinate < range_bottom_y; y_coordinate++) {
                    if (y_coordinate > surface_height + 15 && rand() % 100 < fill_probability)
                        world.blocks[x_coordinate][y_coordinate] = Block(Air);
                }
            }
        }

        void smooth_cave_walls(int range_top_y, int range_bottom_y, int wall_threshold, int surface_safety_margin) {
            std::vector<std::vector<Block>> next_generation_grid = world.blocks;

            for (int x_coordinate = 1; x_coordinate < world.width - 1; x_coordinate++) {
                int surface_height = world.get_surface_height(x_coordinate);
                std::vector<int> layer_transition_offsets = world.get_layer_offsets(x_coordinate);

                for (int y_coordinate = range_top_y + 1; y_coordinate < range_bottom_y - 1; y_coordinate++) {
                    if (y_coordinate <= surface_height + surface_safety_margin || world.blocks[x_coordinate][y_coordinate].type == Grass) continue;

                    int neighboring_wall_count = count_neighboring_walls(x_coordinate, y_coordinate);
                    if (neighboring_wall_count >= wall_threshold) {
                        WorldLayer current_biome_layer = world.get_layer(y_coordinate, surface_height, layer_transition_offsets);
                        next_generation_grid[x_coordinate][y_coordinate] = Block(select_block_by_layer(current_biome_layer, false));
                    } else {
                        next_generation_grid[x_coordinate][y_coordinate] = Block(Air);
                    }
                }
            }
            world.blocks = next_generation_grid;
        }

        void fill_isolated_air_pockets(int range_top_y, int range_bottom_y, int wall_threshold, int surface_safety_margin) {
            std::vector<std::vector<Block>> next_generation_grid = world.blocks;

            for (int x_coordinate = 1; x_coordinate < world.width - 1; x_coordinate++) {
                int surface_height = world.get_surface_height(x_coordinate);
                std::vector<int> layer_transition_offsets = world.get_layer_offsets(x_coordinate);

                for (int y_coordinate = range_top_y + 1; y_coordinate < range_bottom_y - 1; y_coordinate++) {
                    if (y_coordinate <= surface_height + surface_safety_margin || world.blocks[x_coordinate][y_coordinate].type != Air) continue;

                    if (count_neighboring_walls(x_coordinate, y_coordinate) >= wall_threshold) {
                        WorldLayer current_biome_layer = world.get_layer(y_coordinate, surface_height, layer_transition_offsets);
                        next_generation_grid[x_coordinate][y_coordinate] = Block(select_block_by_layer(current_biome_layer, true));
                    }
                }
            }
            world.blocks = next_generation_grid;
        }

        BlockType select_block_by_layer(WorldLayer biome_layer, bool prefer_deep_variant) {
            if (biome_layer == UNDERWORLD) return Hellstone;
            if (biome_layer == CAVERN) return prefer_deep_variant ? Deepstone : Stone;
            return Dirt;
        }

        int count_neighboring_walls(int center_x, int center_y) {
            int wall_count = 0;
            for (int offset_x = -1; offset_x <= 1; offset_x++) {
                for (int offset_y = -1; offset_y <= 1; offset_y++) {
                    if (offset_x == 0 && offset_y == 0) continue;
                    if (world.blocks[center_x + offset_x][center_y + offset_y].type != Air) wall_count++;
                }
            }
            return wall_count;
        }
};
