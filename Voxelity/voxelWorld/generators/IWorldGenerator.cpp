//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

void IWorldGenerator::generate(Chunk& chunk) {
    const auto globalChunkPos = chunk.getPosition() * Chunk::SIZE;
    for (int x = 0; x < Chunk::SIZE; x++) {
        for (int y = 0; y < Chunk::SIZE; y++) {
            for (int z = 0; z < Chunk::SIZE; z++) {
                auto globalVoxelPos = globalChunkPos + glm::ivec3(x, y, z);
                const BlockType blockType = generateVoxel(globalVoxelPos);
                chunk.set(x, y, z, blockType);
            }
        }
    }
}
