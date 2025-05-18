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

/**
 * @brief Mesh builder that generates visible voxel faces using face culling.
 *
 * This implementation only generates faces of voxels that are visible,
 * i.e., faces that are not occluded by neighboring solid voxels.
 */
class FaceCullingMeshBuilder final : public IChunkMeshBuilder {
public:

    /**
    * @brief Generates the mesh (list of visible voxel faces) for a chunk neighborhood.
    *
    * @param neighborhood The chunk data neighborhood containing the center chunk and its neighbors.
    * @return std::vector<VoxelFace> List of visible voxel faces to be rendered.
    */
    std::vector<VoxelFace> mesh(const ChunkDataNeighborhood& neighborhood) override {
        const std::vector<VoxelFace> faces = generateFaceInstances(neighborhood);
        return faces;
    }

    /**
     * @brief Determines if a face of a voxel is visible, i.e., not occluded by a solid neighbor.
     *
     * This checks the voxel in the given direction relative to the current voxel.
     * It handles cases where the neighbor voxel is inside the same chunk or in a neighboring chunk.
     *
     * @param x Local x coordinate of the voxel in the center chunk.
     * @param y Local y coordinate of the voxel in the center chunk.
     * @param z Local z coordinate of the voxel in the center chunk.
     * @param neighborhood The chunk data neighborhood containing the center and neighbors.
     * @param direction The face direction to check visibility against.
     * @return true If the face is visible (neighbor voxel is empty or out of bounds).
     * @return false If the face is occluded by a solid neighbor voxel.
     */
    static bool isFaceVisible(const int x, const int y, const int z, const ChunkDataNeighborhood& neighborhood, const BlockFace direction) {
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

    /**
     * @brief Generates a list of visible voxel faces for the entire chunk neighborhood.
     *
     * Iterates through all voxels in the center chunk, and for each solid voxel,
     * checks each face direction to determine if it should be included in the mesh.
     *
     * @param neighborhood The chunk data neighborhood (center + neighbors).
     * @return std::vector<VoxelFace> Vector of visible voxel faces.
     */
    static std::vector<VoxelFace> generateFaceInstances(const ChunkDataNeighborhood& neighborhood) {
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
};

#endif //FACECULLINGMESHER_H
