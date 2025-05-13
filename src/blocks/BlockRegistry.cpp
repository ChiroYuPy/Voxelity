//
// Created by adrian on 13/05/25.
//

#include "../../include/blocks/BlockRegistry.h"

const std::array<BlockData, static_cast<size_t>(BlockType::COUNT)> BlockRegistry::data = {{
    { BlockType::Air,   false, {0.0f, 0.0f, 0.0f} },
    { BlockType::Grass, true,  {0.2f, 0.8f, 0.2f} },
    { BlockType::Dirt,  true,  {0.5f, 0.3f, 0.1f} },
    { BlockType::Stone, true,  {0.5f, 0.5f, 0.5f} },
}};

const BlockData& BlockRegistry::get(BlockType type) {
    return data[static_cast<size_t>(type)];
}
