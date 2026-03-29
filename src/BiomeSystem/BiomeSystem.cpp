#include "splashkit.h"
#include "BiomeSystem.h"

std::map<BiomeType, Biome> biome_data = {
    {BiomeType::Plains, {Grass, Dirt, Stone, Cavernstone, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Desert, {Sand, Sand, Sandstone, Sandstone, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Snow, {Snow, Snow, Ice, Ice, Hellstone, UndergroundWall, CavernWall, HellWall}},
    {BiomeType::Jungle, {JungleGrass, JungleDirt, Stone, Stone, Hellstone, UndergroundWall, CavernWall, HellWall}}
};

std::vector<BiomeType> generate_biome_map(int width, unsigned int seed)
{
    std::vector<BiomeType> map(width);

    srand(seed);

    int x = 0;

    while (x < width)
    {
        BiomeType biome = static_cast<BiomeType>(rand() % 4);
        int size = rnd(100, 300);

        for (int i = 0; i < size && x < width; i++, x++)
        {
            map[x] = biome;
        }
    }

    return map;
}