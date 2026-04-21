#include "World.h"
#include "../Block/Block.h"
#include "../BiomeSystem/BiomeSystem.h"
#include "../PerlinNoise/PerlinNoise.h"
#include "../TerrainSnakeGenerator/TerrainSnakeGenerator.h"
#include "splashkit.h"

/*
    Author: Lashen Dharmadasa
    
    Description: 
        This system generates the 2D terrain for this game, using Perlin Noise.
        It has a lot of configuration variables, for my experimentation during this project.
        It's crude in that it completes the first and second pass for biomes and simple generation.
        Tunnelling and other features come later on.
*/

// World Constructor
World::World(int w, int h, unsigned int s, int z, const WorldConfig& cfg) 
    : width(w), height(h), seed(s), zoom(z), config(cfg), noise_surface(s + cfg.noise.surface_offset), noise_biome(s + 2000)
{
    surface_base = height * config.terrain.base_height_ratio;
    blocks.assign(width, std::vector<Block>(height, Block(Air)));
}

// Pass 1 of the generation
// Fills the world with the majority block for every layer to createe the baseline
void World::generate_base_terrain() {
    #pragma omp parallel for // To run this in a seperate thread for speed
    for (int x = 0; x < width; x++) {
        surface_map[x] = get_surface_height(x);
        
        // Compute layer limits for this column
        int current_y = surface_map[x];
        layer_limits[x].resize(config.layers.size());
        for (size_t i = 0; i < config.layers.size(); i++) {
            const LayerConfig& layer = config.layers[i];
            double wiggle = noise_surface.value({ (double)x * layer.noise_scale, (double)seed + (double)i * 100.0 }) * layer.variation;
            current_y += layer.thickness + (int)wiggle;
            layer_limits[x][i] = current_y;
        }

        for (int y = 0; y < height; y++) {
            if (y < surface_map[x]) {
                blocks[x][y] = Block(Air);
                continue;
            }

            int layer_idx = get_layer_index_at(x, y); 
            Biome& biome = get_biome_data().at(biome_map[x]);
            const std::vector<BlockOption>* options = get_options_for_layer(biome, layer_idx);

            // Identify the base block, which is the block with the highest weight
            BlockType base_type = options -> front().type;
            float max_w = -1.0f;
            for (size_t i = 0; i < options->size(); ++i) {
                const BlockOption& opt = (*options)[i];
                if (opt.weight > max_w) {
                    max_w = opt.weight;
                    base_type = opt.type;
                }
            }

            blocks[x][y] = Block(base_type, Solid, get_wall_type_for_biome(biome_map[x], layer_idx));
        }
    }
}

// Pass 2 Drunkard's Walk detail fill for generation
// I used inspiration from this website:
// https://blog.jrheard.com/procedural-dungeon-generation-drunkards-walk-in-clojurescript

void World::generate_secondary_fill() {
    TerrainSnakeGenerator::generate(*this);
}

// Generates the terrain using the passes
void World::generate() {
    biome_map = generate_biome_map(width, seed);
    surface_map.resize(width);
    layer_limits.resize(width);

    generate_base_terrain();
    generate_secondary_fill();
}

void World::draw(float cam_x, float cam_y)
{
    int tile_size = BLOCK_SIZE * zoom;

    int start_x = cam_x / tile_size;
    int end_x   = (cam_x + screen_width()) / tile_size + 1;

    int start_y = cam_y / tile_size;
    int end_y   = (cam_y + screen_height()) / tile_size + 1;

    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x > width) end_x = width;
    if (end_y > height) end_y = height;

    for (int x = start_x; x < end_x; x++)
    {
        for (int y = start_y; y < end_y; y++)
        {
            const Block& block = blocks[x][y];

            int bx = x * tile_size - cam_x;
            int by = y * tile_size - cam_y;

            if (block.type != Air)
            {
                fill_rectangle(block_colors[block.type], bx, by, tile_size, tile_size);
            }
            else if (block.wall != AirWall)
            {
                fill_rectangle(wall_colors[block.wall], bx, by, tile_size, tile_size);
            }
        }
    }
}

// Finds layer at a specific position
int World::get_layer_index_at(int x, int y) {
    if (y < surface_map[x]) return -1;
    const std::vector<int>& limits = layer_limits[x];
    for (int i = 0; i < (int) limits.size(); i++) {
        if (y < limits[i]) return i;
    }
    return (int) config.layers.size(); // If no layer can be found then return last layer
}

WallType World::get_wall_type_for_biome(BiomeType type, int layer_index) {
    Biome& biome = get_biome_data().at(type);
    if (layer_index == 0) return AirWall;
    if (layer_index <= 2) return biome.underground_wall;
    if (layer_index == 3) return biome.cavern_wall;
    return biome.underworld_wall;
}

Block World::get_block_at(int x, int y, BiomeType biome_type, const std::vector<int>& layer_limit) {
    if (x < 0 || x >= width || y < 0 || y >= height){
        return Block(Air);
    }
    int layer_idx = get_layer_index_at(x, y); 
    Biome& biome = get_biome_data().at(biome_type);
    const std::vector<BlockOption>* options = get_options_for_layer(biome, layer_idx);
    BlockType type = Air;
    if (options && !options -> empty()) {
        type = options -> front().type;
    }
    return Block(type, Solid, get_wall_type_for_biome(biome_type, layer_idx));
}


int World::get_surface_height(int x) {
    double amp = (double)height * config.terrain.amplitude_ratio;
    double big = noise_surface.value({ (double)x * config.terrain.big_scale, (double)seed });
    double small = noise_surface.value({ (double)x * config.terrain.small_scale, (double)seed * 2.0 });

    return (int)((double)surface_base + (big * amp) + (small * amp * config.terrain.small_weight));
}

std::vector<BlockOption>* World::get_options_for_layer(Biome& biome, int layer_index) {
    switch (layer_index) {
        case 0: return &biome.surface;
        case 1: return &biome.subsurface;
        case 2: return &biome.underground;
        case 3: return &biome.cavern;
        default: return &biome.underworld;
    }
}

// Player Connection
point_2d World::get_random_spawn_point() {
    const int max_attempts = 1000;

    for (int i = 0; i < max_attempts; i++) {
        int x = rand() % width;
        int y = surface_map[x];

        if (y > 0 && y < height - 1 && blocks[x][y].type != Air && blocks[x][y - 2].type == Air) {
            return { (double) x * BLOCK_SIZE * zoom, (double) (y - 2) * BLOCK_SIZE * zoom };
        }
    }

    int x = width / 2;
    int y = surface_map[x];
    return { (double) x * BLOCK_SIZE * zoom, (double) (y - 2) * BLOCK_SIZE * zoom };
}

void World::remove_block(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        blocks[x][y].type = Stone;
    }
}

bool World::is_solid(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;

    return blocks[x][y].type != Air;
}