//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

void IWorldGenerator::generate(Chunk& chunk) {
    const auto globalChunkPos = chunk.getPosition() * CHUNK_SIZE;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                auto globalVoxelPos = globalChunkPos + glm::ivec3(x, y, z);
                const BlockType blockType = generateVoxel(globalVoxelPos);
                chunk.set(x, y, z, blockType);
            }
        }
    }
}
