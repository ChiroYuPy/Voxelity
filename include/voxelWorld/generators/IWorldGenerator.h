//
// Created by adrian on 14/05/25.
//

#ifndef IWORLDGENERATOR_H
#define IWORLDGENERATOR_H

#include "voxelWorld/Chunk.h"

class IWorldGenerator {
public:
    virtual ~IWorldGenerator() = default;

    virtual BlockType generateVoxel(const glm::ivec3& globalPos) = 0;

    void generate(Chunk& chunk);
};

#endif //IWORLDGENERATOR_H
