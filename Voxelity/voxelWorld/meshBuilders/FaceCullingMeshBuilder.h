//
// Created by adrian on 18/05/25.
//

#ifndef FACECULLINGMESHER_H
#define FACECULLINGMESHER_H

#include <vector>

#include "IChunkMeshBuilder.h"
#include "core/Constants.h"
#include "math/BlockFace.h"
#include "../chunk/VoxelFace.h"
#include "voxelWorld/chunk/Chunk.h"
#include "voxelWorld/chunk/Voxel.h"

class FaceCullingMeshBuilder final : public IChunkMeshBuilder {
public:
    std::vector<VoxelFace> mesh(Chunk& chunk) override {
        const std::vector<VoxelFace> faces = generateFaceInstances(chunk);
        return faces;
    }

    static bool isFaceVisible(const int x, const int y, const int z, const Chunk& chunk, const BlockFace direction) {
        const glm::ivec3 offset = DIRECTION_NORMALS[static_cast<int>(direction)];
        glm::ivec3 neighborPos = { x + offset.x, y + offset.y, z + offset.z };

        const Chunk *targetChunk = &chunk;

        if (neighborPos.x < 0 || neighborPos.x >= Constants::ChunkSize ||
            neighborPos.y < 0 || neighborPos.y >= Constants::ChunkSize ||
            neighborPos.z < 0 || neighborPos.z >= Constants::ChunkSize) {

            targetChunk = chunk.getNeighbor(direction);
            if (!targetChunk) return true;

            neighborPos.x = (neighborPos.x + Constants::ChunkSize) % Constants::ChunkSize;
            neighborPos.y = (neighborPos.y + Constants::ChunkSize) % Constants::ChunkSize;
            neighborPos.z = (neighborPos.z + Constants::ChunkSize) % Constants::ChunkSize;
            }

        const Voxel voxel = targetChunk->getData().get(neighborPos.x, neighborPos.y, neighborPos.z);
        return !voxel.isSolid();
    }

    static std::vector<VoxelFace> generateFaceInstances(const Chunk& chunk) {
        std::vector<VoxelFace> faces;

        for (int z = 0; z < Constants::ChunkSize; ++z)
            for (int y = 0; y < Constants::ChunkSize; ++y)
                for (int x = 0; x < Constants::ChunkSize; ++x) {
                    const Voxel* voxel = &chunk.getData().get(x, y, z);
                    if (!voxel->isSolid()) continue;

                    for (const BlockFace direction : DIRECTIONS) {
                        if (isFaceVisible(x, y, z, chunk, direction)) {
                            faces.emplace_back(glm::ivec3(x, y, z), static_cast<int>(direction), static_cast<int>(voxel->getType()));
                        }
                    }
                }

        return faces;
    }
};

#endif //FACECULLINGMESHER_H
