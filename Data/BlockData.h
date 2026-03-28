#pragma once
#include "splashkit.h"
#include <map>

const int BLOCK_SIZE = 2;

enum BlockType {
    Air,
    Grass,
    Dirt,
    Stone,
    Hellstone,
    Deepstone
};

enum BlockState {
    Solid
};

struct Block {
    BlockType type;
    BlockState state;

    Block(BlockType type_value, BlockState state_value) : type(type_value), state(state_value) {}
};

std::map<BlockType, color> block_colors = {
    {Air, COLOR_LIGHT_BLUE}, {Grass, COLOR_GREEN}, {Dirt, COLOR_BROWN}, {Stone, COLOR_GRAY}, {Hellstone, COLOR_ORANGE}, {Deepstone, COLOR_DARK_GRAY}
};