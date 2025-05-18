//
// Created by adrian on 14/05/25.
//

#ifndef IWORLDGENERATOR_H
#define IWORLDGENERATOR_H

#include "blocks/BlockType.h"

struct ChunkData;
class Chunk;

class IChunkGenerator {
public:
    virtual ~IChunkGenerator() = default;

    virtual BlockType generateVoxel(const glm::ivec3& globalPos) = 0;

    ChunkData generate(glm::ivec3 globalChunkPos);
};

#endif //IWORLDGENERATOR_H
