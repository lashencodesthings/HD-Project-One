#include "splashkit.h"
#include "BiomeSystem.h"

std::map<BiomeType, Biome> biome_data = {
    {BiomeType::Plains, {Grass, Dirt, Stone, Cavernstone, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Desert, {Sand, Sand, Sandstone, Cavernstone, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Snow, {Snow, Snow, Ice, Cavernstone, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Jungle, {JungleGrass, JungleDirt, Stone, Cavernstone, Hellstone, UndergroundWall, CavernWall, HellWall}}
};

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