#pragma once
#include "splashkit.h"
#include <map>

const int BLOCK_SIZE = 2;

enum WallType
{
    AirWall,
    UndergroundWall,
    CavernWall,
    HellWall,      
};

enum BlockType {
    Air,
    Grass,
    Dirt,
    Stone,
    Hellstone,
    Cavernstone
};

enum BlockState {
    Solid
};

struct Block {
    BlockType type;
    BlockState state;
    WallType back_wall;

    Block(BlockType type_value, BlockState state_value, WallType wall_value) : type(type_value), state(state_value), back_wall(wall_value) {}
    Block(BlockType type_value) : type(type_value), state(Solid), back_wall(AirWall) {}
};

std::map<BlockType, color> block_colors = {
    {Air, COLOR_LIGHT_BLUE}, 
    {Grass, COLOR_GREEN}, 
    {Dirt, rgb_color(123, 88, 62)}, 
    {Stone, COLOR_GRAY}, 
    {Hellstone, rgb_color(70, 65, 75)}, 
    {Cavernstone, rgb_color(125, 126, 121)}, 
};

std::map<WallType, color> wall_colors = {
    {UndergroundWall, rgb_color(71, 49, 39)},
    {CavernWall, rgb_color(69, 63, 55)},
    {HellWall, rgb_color(48, 0, 0)},

};