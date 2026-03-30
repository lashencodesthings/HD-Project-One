#include "Block.h"

// Construct the global colour maps
std::map<BlockType, color> block_colors = {
    {Air, COLOR_LIGHT_BLUE},
    {Grass, COLOR_GREEN},
    {Dirt, rgb_color(123,88,62)},
    {Stone, COLOR_GRAY},
    {Sand, rgb_color(219,212,135)},
    {Sandstone, rgb_color(188,170,86)},
    {Hellstone, rgb_color(70,65,75)},
    {Cavernstone, rgb_color(69, 66, 88)},
    {Snow, rgb_color(255, 255, 255)},
    {Ice, rgb_color(168,200,216)},
    {JungleDirt, rgb_color(73,54,29)},
    {JungleGrass, rgb_color(150,216,43)},
    {JungleStone, rgb_color(70, 63, 55)}
};

std::map<WallType, color> wall_colors = {
    {UndergroundWall, rgb_color(71,49,39)},
    {CavernWall, rgb_color(36, 32, 52)},
    {HellWall, rgb_color(48,0,0)},
    {SandWall, rgb_color(184, 126, 59)},
    {SnowWall, rgb_color(127, 134, 137)},
    {JungleWall, rgb_color(53, 80, 30)}
};

// Constructors for blocks
Block::Block(BlockType type_value, BlockState state_value, WallType wall_value)
    : type(type_value), state(state_value), wall(wall_value) {}

Block::Block(BlockType type_value)
    : type(type_value), state(Solid), wall(AirWall) {}

Block::Block(WallType wall_value)
    : state(Solid), wall(wall_value)
{
    switch (wall_value) {
        case UndergroundWall: type = Dirt; break;
        case CavernWall: type = Stone; break;
        case HellWall: type = Hellstone; break;
        case SandWall: type = Sand; break;
        case SnowWall: type = Snow; break;
        case JungleWall: type = JungleGrass; break;
        case AirWall:
        default: type = Air; break;
    }
}