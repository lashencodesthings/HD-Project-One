#pragma once
#include "splashkit.h"
#include "../Data/WorldGenerationHeaders.h"
#include <random>

class CellularAutomata {
public:
    CellularAutomata(World &target_world) : world(target_world), rng(target_world.seed) {}

    void run(int range_top_y, int range_bottom_y, int evolution_iterations, int initial_fill_probability) {
        seed_initial_noise_pockets(range_top_y, range_bottom_y, initial_fill_probability);

        for (int i = 0; i < evolution_iterations; i++) {
            smooth_cave_walls(range_top_y, range_bottom_y, DEFAULT_WALL_THRESHOLD, DEFAULT_SURFACE_MARGIN);
        }

        smooth_cave_walls(range_top_y, range_bottom_y, DEFAULT_WALL_THRESHOLD, REDUCED_SURFACE_MARGIN);
        fill_isolated_air_pockets(range_top_y, range_bottom_y, POCKET_FILL_THRESHOLD);
        smooth_cave_walls(range_top_y, range_bottom_y, FINAL_WALL_THRESHOLD, REDUCED_SURFACE_MARGIN);
    }

private:
    World &world;
    std::mt19937 rng;

    static const int DEFAULT_WALL_THRESHOLD = 5;
    static const int FINAL_WALL_THRESHOLD = 6;
    static const int POCKET_FILL_THRESHOLD = 7;

    static const int DEFAULT_SURFACE_MARGIN = 5;
    static const int REDUCED_SURFACE_MARGIN = 3;

    static const int SURFACE_CARVE_DEPTH = 3;
    static const int SURFACE_CARVE_CHANCE = 30;

    int random_percent() {
        static std::uniform_int_distribution<int> dist(0, 99);
        return dist(rng);
    }

    bool is_solid_block(const Block &block) {
        return block.type != Air;
    }

    WallType wall_from_layer(WorldLayer layer, int y, int surface_height) {
        if (y <= surface_height) {
            return AirWall;
        }

        switch (layer) {
            case SURFACE:
            case UNDERGROUND:
                return UndergroundWall;
            case CAVERN:
                return CavernWall;
            case UNDERWORLD:
                return HellWall;
            default:
                return AirWall;
        }
    }

    void seed_initial_noise_pockets(int range_top_y, int range_bottom_y, int fill_probability) {
        for (int x = 0; x < world.width; x++) {
            int surface = world.get_surface_height(x);

            for (int y = range_top_y; y < range_bottom_y; y++) {
                if (y <= surface) continue;

                if (random_percent() < fill_probability) {
                    WorldLayer layer = world.get_layer(x, y, surface);
                    world.blocks[x][y] = Block(Air, Solid, wall_from_layer(layer, y, surface));
                }
            }
        }
    }

    void smooth_cave_walls(int range_top_y, int range_bottom_y, int wall_threshold, int surface_margin) {
        std::vector<std::vector<Block>> next_generation_grid = world.blocks;

        for (int x = 1; x < world.width - 1; x++) {
            int surface = world.get_surface_height(x);

            for (int y = range_top_y + 1; y < range_bottom_y - 1; y++) {
                if (y < surface) continue;

                int neighbor_count = count_neighboring_walls(x, y);
                WorldLayer layer = world.get_layer(x, y, surface);

                if (neighbor_count >= wall_threshold) {
                    next_generation_grid[x][y] = Block(
                        select_block_by_layer(layer, y, surface, false),
                        Solid,
                        wall_from_layer(layer, y, surface)
                    );
                } else {
                    WallType cave_wall = (y == surface) ? UndergroundWall : wall_from_layer(layer, y, surface);
                    next_generation_grid[x][y] = Block(Air, Solid, cave_wall);
                }
            }
        }
        world.blocks = next_generation_grid;
    }

    void fill_isolated_air_pockets(int range_top_y, int range_bottom_y, int wall_threshold) {
        std::vector<std::vector<Block>> next_generation_grid = world.blocks;

        for (int x = 1; x < world.width - 1; x++) {
            int surface = world.get_surface_height(x);

            for (int y = range_top_y + 1; y < range_bottom_y - 1; y++) {
                if (y < surface) continue;

                int distance_from_surface = y - surface;
                bool carve_surface =
                    distance_from_surface >= 0 &&
                    distance_from_surface < SURFACE_CARVE_DEPTH &&
                    random_percent() < SURFACE_CARVE_CHANCE;

                if (count_neighboring_walls(x, y) >= wall_threshold && !carve_surface) {
                    WorldLayer layer = world.get_layer(x, y, surface);

                    next_generation_grid[x][y] = Block(
                        select_block_by_layer(layer, y, surface, true),
                        Solid,
                        wall_from_layer(layer, y, surface)
                    );
                }
            }
        }

        world.blocks = next_generation_grid;
    }

    BlockType select_block_by_layer(WorldLayer layer, int y, int surface, bool prefer_deep_variant) {
        if (layer == SURFACE) {
            return (y == surface) ? Grass : Dirt;
        }

        if (layer == UNDERWORLD) return Hellstone;
        if (layer == CAVERN) return prefer_deep_variant ? Cavernstone : Stone;

        return Dirt;
    }

    int count_neighboring_walls(int center_x, int center_y) {
        int count = 0;
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0) continue;
                if (is_solid_block(world.blocks[center_x + x][center_y + y])) {
                    count++;
                }
            }
        }

        return count;
    }
};