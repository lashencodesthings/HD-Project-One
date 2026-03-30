#include "../Block/Block.h"

struct BlockOption {
    BlockType type;
    float weight;
};

BlockType pick_weighted(const std::vector<BlockOption>& options, float r);
BlockType pick_weighted_block(const std::vector<BlockOption>& palette);