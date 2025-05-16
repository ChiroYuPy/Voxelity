//
// Created by adrian on 13/05/25.
//

#ifndef BLOCKREGISTERY_H
#define BLOCKREGISTERY_H

#include <GLT.h>

#include "BlockType.h"
#include <array>

struct BlockData {
    BlockType type;
    bool solid;
};

class BlockRegistry {
public:
    static const BlockData &get(BlockType type);

    static BlockType fromID(uint8_t id);

    static bool isSolid(BlockType type);
    static bool isVoid(BlockType type);

private:
    static const std::array<BlockData, static_cast<size_t>(BlockType::COUNT)> data;
};

#endif //BLOCKREGISTERY_H
