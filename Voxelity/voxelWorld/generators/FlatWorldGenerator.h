//
// Created by adrian on 14/05/25.
//

#ifndef FLATWORLDGENERATOR_H
#define FLATWORLDGENERATOR_H

#include "IWorldGenerator.h"

class FlatWorldGenerator final : public IChunkGenerator {
public:
    BlockType generateVoxel(const glm::ivec3& position) override;
};

#endif //FLATWORLDGENERATOR_H
