#include "splashkit.h"
#include "BiomeSystem.h"

#include "splashkit.h"
#include "BiomeSystem.h"

std::map<BiomeType, Biome>& get_biome_data()
{
    static std::map<BiomeType, Biome> biome_data = {

        {BiomeType::Plains, {
            // Surface
            { 
                {Grass, 0.85f}, 
                {Dirt,  0.15f} 
            },

            // Subsurface 
            {
                {Dirt,  1.0f},
            },

            // Underground
            {
                {Dirt,  0.30f},
                {Stone, 0.70f}
            },

            // Cavern
            {
                {Stone, 0.25f},
                {Cavernstone, 0.75f}
            },

            // Underworld
            {
                {Hellstone, 1.0f}
            },

            UndergroundWall,
            CavernWall,
            HellWall
        }},

        {BiomeType::Desert, {
            // Surface
            {
                {Sand, 0.95f},
                {Sandstone, 0.05f}
            },

            // Subsurface
            {
                {Sand, 0.7f},
                {Sandstone, 0.3f}
            },

            // Underground
            {
                {Sandstone, 1.0f}
            },

            // Cavern
            {
                {Cavernstone, 1.0f}
            },

            // Underworld
            {
                {Hellstone, 1.0f}
            },

            SandWall,
            CavernWall,
            HellWall
        }},

        {BiomeType::Snow, {
            // Surface
            {
                {Snow, 0.9f},
                {Ice,  0.1f}
            },

            // Subsurface
            {
                {Snow, 0.6f},
                {Ice,  0.4f}
            },

            // Underground
            {
                {Ice, 1.0f}
            },

            // Cavern
            {
                {Cavernstone, 1.0f}
            },

            // Underworld
            {
                {Hellstone, 1.0f}
            },

            SnowWall,
            CavernWall,
            HellWall
        }},

        {BiomeType::Jungle, {
            // Surface
            {
                {JungleGrass, 0.85f},
                {JungleDirt,  0.15f}
            },

            // Subsurface
            {
                {JungleDirt,  0.75f},
                {JungleStone, 0.25f}
            },

            // Underground
            {
                {JungleStone, 1.0f}
            },

            // Cavern
            {
                {Cavernstone, 1.0f}
            },

            // Underworld
            {
                {Hellstone, 1.0f}
            },

            JungleWall,
            CavernWall,
            HellWall
        }}
    };

    return biome_data;
}

std::vector<BiomeType> sequence = {
    BiomeType::Plains,
    BiomeType::Desert,
    BiomeType::Plains,
    BiomeType::Snow,
    BiomeType::Plains,
    BiomeType::Jungle
};

std::vector<BiomeType> generate_biome_map(int width, unsigned int seed)
{
    std::vector<BiomeType> map(width);

    srand(seed);

    int x = 0;

    for (BiomeType biome : sequence)
    {
        int size = 100 + rand() % 101;

        for (int i = 0; i < size && x < width; i++, x++)
        {
            map[x] = biome;
        }

        if (x >= width) break;
    }

    while (x < width)
    {
        map[x] = sequence.back();
        x++;
    }

    return map;
} 