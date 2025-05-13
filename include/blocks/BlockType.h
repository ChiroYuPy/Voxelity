//
// Created by adrian on 13/05/25.
//

#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <GLT.h>

enum class BlockType : glm::uint8_t {
    Air = 0, // default block
    Grass,
    Dirt,
    Stone,
    COUNT    // count of block types
};

#endif //BLOCKTYPE_H
