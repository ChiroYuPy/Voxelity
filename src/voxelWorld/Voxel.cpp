//
// Created by adrian on 13/05/25.
//

#include "voxelWorld/Voxel.h"

#include "blocks/BlockRegistry.h"

Voxel::Voxel() {}

Voxel::Voxel(const uint8_t id) {
    type = BlockRegistry::fromID(id);
}

bool Voxel::isSolid() const {
    return BlockRegistry::get(type).solid;
}