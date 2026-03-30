#pragma once
#include "splashkit.h"
#include <map>

// Enumerations
enum WallType { AirWall, SandWall, SnowWall, JungleWall, UndergroundWall, CavernWall, HellWall };
enum BlockType { Air, Grass, Dirt, Stone, Sand, Snow, Ice, JungleDirt, JungleGrass, JungleStone, Sandstone, Hellstone, Cavernstone };
enum BlockState { Solid };

// Block struct
struct Block {
    BlockType type;
    BlockState state;
    WallType wall;

    // Constructor for the blocks
    Block(BlockType type_value, BlockState state_value, WallType wall_value);
    Block(BlockType type_value);
    Block(WallType wall_value);
};

// Global color maps
extern std::map<BlockType, color> block_colors;
extern std::map<WallType, color> wall_colors;