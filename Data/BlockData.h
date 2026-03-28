#pragma once
#include "splashkit.h"
#include <map>

const int BLOCK_SIZE = 16;

enum BlockType {
    Air,
    Grass,
    Dirt,
    Stone
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
    {Air, COLOR_WHITE}, {Grass, COLOR_GREEN}, {Dirt, COLOR_BROWN}, {Stone, COLOR_GRAY}
};