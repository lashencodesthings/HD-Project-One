#include "BlockOption.h"
#include "../Block/Block.h"

BlockType pick_weighted(const std::vector<BlockOption>& options, float r)
{
    if (options.empty()) return Air;

    float total_weight = 0.0f;
    for (size_t i = 0; i < options.size(); ++i) {
        total_weight += options[i].weight;
    }

    float target = r * total_weight;
    float cumulative = 0.0f;

    for (size_t i = 0; i < options.size(); ++i) {
        cumulative += options[i].weight;
        if (target <= cumulative) {
            return options[i].type;
        }
    }

    return options.back().type;
}