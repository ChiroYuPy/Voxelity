//
// Created by adrian on 14/05/25.
//

#ifndef NATURALWORLDGENERATOR_H
#define NATURALWORLDGENERATOR_H

#include "IWorldGenerator.h"

class NaturalWorldGenerator final : public IWorldGenerator {
public:
    NaturalWorldGenerator();

    BlockType generateVoxel(const glm::ivec3& position) override;
};

#endif //NATURALWORLDGENERATOR_H
