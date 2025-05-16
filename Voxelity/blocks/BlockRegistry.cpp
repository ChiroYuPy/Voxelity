//
// Created by adrian on 13/05/25.
//

#include "blocks/BlockRegistry.h"

const std::array<BlockData, static_cast<size_t>(BlockType::COUNT)> BlockRegistry::data = {{
    { BlockType::Air,       false },
    { BlockType::Grass,     true  },
    { BlockType::Dirt,      true  },
    { BlockType::Stone,     true  },
    { BlockType::Bedrock,   true  }
}};

const BlockData& BlockRegistry::get(BlockType type) {
    return data[static_cast<size_t>(type)];
}

BlockType BlockRegistry::fromID(uint8_t id) {
    if (id >= static_cast<uint8_t>(BlockType::COUNT)) return BlockType::Air;
    return static_cast<BlockType>(id);
}

bool BlockRegistry::isSolid(const BlockType type) {
    return get(type).solid;
}

bool BlockRegistry::isVoid(const BlockType type) {
    return type == BlockType::Air;
}