//
// Created by adrian on 13/05/25.
//

#include "Voxel.h"

#include "../include/blocks/BlockRegistry.h"

[[nodiscard]] bool Voxel::isSolid() const {
    return BlockRegistry::get(type).solid;
}
