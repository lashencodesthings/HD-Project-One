#include "TerrainSnakeGenerator.h"
#include "../World/World.h"

namespace TerrainSnakeGenerator {
    
    const int VEIN_SPACING = 10;
    const int VEINS_PER_CLUSTER = 10;
    const int VEIN_COUNT_VARIATION = 5;
    const int MIN_VEIN_LENGTH = 100;
    const int VEIN_LENGTH_VARIATION = 300;
    const int MIN_BRUSH_RADIUS = 1;
    const int BRUSH_RADIUS_VARIATION = 2;
    const int MOVE_RANGE = 5;
    const int MOVE_OFFSET = 2;
    const float STEP_SCALE = 1.1f;
    const int SURFACE_OFFSET = 0;
    const int BRANCH_PROBABILITY = 2;
    const int MAX_BRANCHES = 4;

    static std::vector<BlockOption> build_secondary_block_palette(const std::vector<BlockOption>& layer_options);
    static BlockType select_weighted_block(const std::vector<BlockOption>& palette);
    static void apply_brush(World& world, int center_x, int center_y, int radius, const std::vector<BlockOption>& palette, BiomeType origin_biome);
    static void simulate_vein(
        World& world,
        float pos_x,
        float pos_y,
        int lifespan,
        int radius,
        const std::vector<BlockOption>& palette,
        BiomeType origin_biome,
        int& active_vein_count,
        int& branch_count
    );

    void generate(World& world) {
        for (int spawn_x = 0; spawn_x < world.width; spawn_x += VEIN_SPACING) {

            int active_vein_count = VEINS_PER_CLUSTER + (rand() % VEIN_COUNT_VARIATION);
            int branch_count = 0;

            for (int i = 0; i < active_vein_count; i++) {

                float start_x = (float)(spawn_x + (rand() % (VEIN_SPACING / 2)) - (VEIN_SPACING / 4));
                int clamped_x = (int)std::max(0.0f, std::min((float)world.width - 1, start_x));
                float start_y = (float)(world.surface_map[clamped_x] + SURFACE_OFFSET + (rand() % 100));

                int lifespan = MIN_VEIN_LENGTH + (rand() % VEIN_LENGTH_VARIATION);
                int radius = MIN_BRUSH_RADIUS + (rand() % BRUSH_RADIUS_VARIATION);

                int origin_layer = world.get_layer_index_at(clamped_x, (int)start_y);
                BiomeType origin_biome = world.biome_map[clamped_x];

                Biome& biome = get_biome_data().at(origin_biome);
                const std::vector<BlockOption>* layer_options = world.get_options_for_layer(biome, origin_layer);

                if (layer_options == NULL || layer_options->empty()) continue;

                std::vector<BlockOption> palette = build_secondary_block_palette(*layer_options);
                if (palette.empty()) continue;

                simulate_vein(
                    world,
                    start_x,
                    start_y,
                    lifespan,
                    radius,
                    palette,
                    origin_biome,
                    active_vein_count,
                    branch_count
                );
            }
        }
    }

    static std::vector<BlockOption> build_secondary_block_palette(const std::vector<BlockOption>& layer_options) {
        std::vector<BlockOption> palette;

        BlockType dominant_block = layer_options[0].type;
        float highest_weight = -1.0f;

        for (size_t i = 0; i < layer_options.size(); i++) {
            if (layer_options[i].weight > highest_weight) {
                highest_weight = layer_options[i].weight;
                dominant_block = layer_options[i].type;
            }
        }

        for (size_t i = 0; i < layer_options.size(); i++) {
            if (layer_options[i].type != dominant_block) {
                palette.push_back(layer_options[i]);
            }
        }

        return palette;
    }

    static BlockType select_weighted_block(const std::vector<BlockOption>& palette) {
        float total_weight = 0.0f;

        for (size_t i = 0; i < palette.size(); i++) {
            total_weight += palette[i].weight;
        }

        float random_value = (float)rand() / ((float)RAND_MAX / total_weight);

        float accumulated = 0.0f;

        for (size_t i = 0; i < palette.size(); i++) {
            accumulated += palette[i].weight;
            if (random_value <= accumulated) {
                return palette[i].type;
            }
        }

        return palette[0].type;
    }

    static void apply_brush(
        World& world,
        int center_x,
        int center_y,
        int radius,
        const std::vector<BlockOption>& palette,
        BiomeType origin_biome
    ) {
        for (int offset_y = -radius; offset_y <= radius; offset_y++) {
            for (int offset_x = -radius; offset_x <= radius; offset_x++) {

                int x = center_x + offset_x;
                int y = center_y + offset_y;

                if (x < 0 || x >= world.width || y < 0 || y >= world.height) continue;

                if ((offset_x * offset_x + offset_y * offset_y) > (radius * radius)) continue;
                if (y <= world.surface_map[x]) continue;

                int layer_index = world.get_layer_index_at(x, y);
                BlockType block_type = select_weighted_block(palette);

                world.blocks[x][y] = Block(
                    block_type,
                    Solid,
                    world.get_wall_type_for_biome(origin_biome, layer_index)
                );
            }
        }
    }

    static void simulate_vein(World& world, float pos_x, float pos_y, int lifespan, int radius, const std::vector<BlockOption>& palette, BiomeType origin_biome, int& active_vein_count, int& branch_count) {
        for (int step = 0; step < lifespan; step++) {

            pos_x += (float)((rand() % MOVE_RANGE) - MOVE_OFFSET) * STEP_SCALE;
            pos_y += (float)((rand() % MOVE_RANGE) - MOVE_OFFSET) * STEP_SCALE;

            if (branch_count < MAX_BRANCHES && (rand() % 100) < BRANCH_PROBABILITY) {
                active_vein_count++;
                branch_count++;
            }

            if (pos_x < 0 || pos_x >= world.width || pos_y < 0 || pos_y >= world.height) break;

            apply_brush(world, (int)pos_x, (int)pos_y, radius, palette, origin_biome);
        }
    }

}