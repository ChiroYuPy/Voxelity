//
// Created by adrian on 13/05/25.
//

#ifndef VOXEL_H
#define VOXEL_H

#include "../blocks/BlockType.h"
#include "blocks/BlockRegistry.h"

struct Voxel {
    BlockType type = BlockType::Air;

    explicit Voxel();
    explicit Voxel(uint8_t id);

    [[nodiscard]] bool isSolid() const;
};

#endif //VOXEL_H
