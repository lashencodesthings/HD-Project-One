#pragma once
#include "splashkit.h"

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
    string color;
};