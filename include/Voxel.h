//
// Created by adrian on 13/05/25.
//

#ifndef VOXEL_H
#define VOXEL_H
#include "blocks/BlockType.h"

struct Voxel {
    BlockType type = BlockType::Air;
    [[nodiscard]] bool isSolid() const;
};

#endif //VOXEL_H
