//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

ChunkData IWorldGenerator::generate(const glm::ivec3 globalChunkPos) {
    auto data = ChunkData();
    for (int x = 0; x < Constants::ChunkSize; x++) {
        for (int y = 0; y < Constants::ChunkSize; y++) {
            for (int z = 0; z < Constants::ChunkSize; z++) {
                auto globalVoxelPos = globalChunkPos + glm::ivec3(x, y, z);
                const BlockType blockType = generateVoxel(globalVoxelPos);
                data.set(x, y, z, Voxel{blockType});
            }
        }
    }
    return data;
}