//
// Created by adrian on 18/05/25.
//

#include "FaceCullingMeshBuilder.h"

#include "ChunkDataNeighborhood.h"
#include "core/Constants.h"
#include "voxelWorld/chunk/ChunkData.h"
#include "voxelWorld/chunk/Voxel.h"

std::vector<VoxelFace> FaceCullingMeshBuilder::mesh(const ChunkDataNeighborhood& neighborhood) {
    const std::vector<VoxelFace> faces = generateFaceInstances(neighborhood);
    return faces;
}

bool FaceCullingMeshBuilder::isFaceVisible(const int x, const int y, const int z, const ChunkDataNeighborhood& neighborhood, const BlockFace direction) {
    const glm::ivec3 offset = DIRECTION_NORMALS[static_cast<int>(direction)];
    glm::ivec3 neighborPos = { x + offset.x, y + offset.y, z + offset.z };

    // Case 1: neighborPos inside center chunk
    if (neighborPos.x >= 0 && neighborPos.x < Constants::ChunkSize &&
        neighborPos.y >= 0 && neighborPos.y < Constants::ChunkSize &&
        neighborPos.z >= 0 && neighborPos.z < Constants::ChunkSize) {
        const Voxel voxel = neighborhood.getCenter()->get(neighborPos.x, neighborPos.y, neighborPos.z);
        return !voxel.isSolid();
        }

    // Case 2: neighborPos outside center chunk, sample neighbor chunk
    const ChunkData* neighborChunk = neighborhood.getNeighbor(direction);
    if (!neighborChunk) return true;

    if (neighborPos.x < 0) neighborPos.x += Constants::ChunkSize;
    if (neighborPos.y < 0) neighborPos.y += Constants::ChunkSize;
    if (neighborPos.z < 0) neighborPos.z += Constants::ChunkSize;

    neighborPos.x %= Constants::ChunkSize;
    neighborPos.y %= Constants::ChunkSize;
    neighborPos.z %= Constants::ChunkSize;

    const Voxel voxel = neighborChunk->get(neighborPos.x, neighborPos.y, neighborPos.z);
    return !voxel.isSolid();
}

std::vector<VoxelFace> FaceCullingMeshBuilder::generateFaceInstances(const ChunkDataNeighborhood& neighborhood) {
    std::vector<VoxelFace> faces;

    for (int z = 0; z < Constants::ChunkSize; ++z)
        for (int y = 0; y < Constants::ChunkSize; ++y)
            for (int x = 0; x < Constants::ChunkSize; ++x) {
                const Voxel* voxel = &neighborhood.getCenter()->get(x, y, z);
                if (!voxel->isSolid()) continue;

                for (const BlockFace direction : DIRECTIONS) {
                    if (isFaceVisible(x, y, z, neighborhood, direction)) {
                        faces.emplace_back(glm::ivec3(x, y, z), static_cast<int>(direction), static_cast<int>(voxel->getType()));
                    }
                }
            }

    return faces;
}