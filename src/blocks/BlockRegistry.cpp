//
// Created by adrian on 13/05/25.
//

#include "blocks/BlockRegistry.h"

const std::array<BlockData, static_cast<size_t>(BlockType::COUNT)> BlockRegistry::data = {{
    { BlockType::Air,       false,  {0.0f, 0.0f, 0.0f} },
    { BlockType::Grass,     true,   {0.2f, 0.8f, 0.2f} },
    { BlockType::Dirt,      true,   {0.5f, 0.3f, 0.1f} },
    { BlockType::Stone,     true,   {0.5f, 0.5f, 0.5f} },
    { BlockType::Bedrock,   true,   {0.1f, 0.1f, 0.1f} }
}};

const BlockData& BlockRegistry::get(BlockType type) {
    return data[static_cast<size_t>(type)];
}

BlockType BlockRegistry::fromID(uint8_t id) {
    if (id >= static_cast<uint8_t>(BlockType::COUNT)) return BlockType::Air;
    return static_cast<BlockType>(id);
}