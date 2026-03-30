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
                {Grass, 1.0f},
            },

            // Subsurface 
            {
                {Dirt,  1.0f},
            },

            // Underground
            {
                {Stone, 0.90f},
                {Dirt, 0.1f},
            },

            // Cavern
            {
                {Cavernstone, 0.7f},
                {Stone, 0.25f},
                {Dirt, 0.05f}
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
                {Sand, 1.0f},
            },

            // Subsurface
            {
                {Sand, 0.7f},
                {Sandstone, 0.25f},
            },

            // Underground
            {
                {Sandstone, 1.0f},
            },

            // Cavern
            {
                {Stone, 0.3f},
                {Cavernstone, 0.7f}
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
                {Snow, 1.0f},
            },

            // Subsurface
            {
                {Snow, 0.8f},
                {Ice,  0.2f},
            },

            // Underground
            {
                {Ice, 0.8f},
                {Stone, 0.2f}
            },

            // Cavern
            {
                {Ice, 0.1f},
                {Stone, 0.1f},
                {Cavernstone, 0.8f}
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
                {JungleDirt,  0.2f},
                {JungleStone, 0.8f}
            },

            // Cavern
            {
                {JungleDirt,  0.1f},
                {JungleStone, 0.2f},
                {Cavernstone, 0.7f}
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

struct BiomeSegment
{
    BiomeType type;
    int width;
};

std::vector<BiomeSegment> sequence = {
    {BiomeType::Plains, 240},
    {BiomeType::Desert, 100},
    {BiomeType::Plains, 240},
    {BiomeType::Snow, 50},
    {BiomeType::Plains, 240},
    {BiomeType::Jungle, 180}
};

std::vector<BiomeType> generate_biome_map(int width, unsigned int seed)
{
    std::vector<BiomeType> map(width);

    srand(seed);

    int x = 0;

    for (BiomeSegment segment : sequence)
    {
        for (int i = 0; i < segment.width && x < width; i++, x++)
        {
            map[x] = segment.type;
        }

        if (x >= width) break;
    }

    while (x < width)
    {
        map[x] = sequence.back().type;
        x++;
    }

    return map;
}