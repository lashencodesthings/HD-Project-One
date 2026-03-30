#include "../Block/Block.h"

struct BlockOption {
    BlockType type;
    float weight;
};

BlockType pick_weighted(const std::vector<BlockOption>& options, float r);