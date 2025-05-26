//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/IWorldGenerator.h"

#include "../chunk/Chunk.h"

VoxelStorage IChunkGenerator::generate(const glm::ivec3 globalChunkPos) {
    auto data = VoxelStorage();
    for (int x = 0; x < Constants::CHUNK_SIZE; x++) {
        for (int y = 0; y < Constants::CHUNK_SIZE; y++) {
            for (int z = 0; z < Constants::CHUNK_SIZE; z++) {
                auto globalVoxelPos = globalChunkPos + glm::ivec3(x, y, z);
                const BlockType blockType = generateVoxel(globalVoxelPos);
                data.set(x, y, z, Voxel{blockType});
            }
        }
    }
    return data;
}