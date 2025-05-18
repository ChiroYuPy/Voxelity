//
// Created by adrian on 18/05/25.
//

#ifndef FACECULLINGMESHER_H
#define FACECULLINGMESHER_H

#include <vector>

#include "IChunkMeshBuilder.h"
#include "math/BlockFace.h"
#include "../chunk/VoxelFace.h"

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
    std::vector<VoxelFace> mesh(const ChunkDataNeighborhood& neighborhood) override;

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
    static bool isFaceVisible(int x, int y, int z, const ChunkDataNeighborhood& neighborhood, BlockFace direction);

    /**
     * @brief Generates a list of visible voxel faces for the entire chunk neighborhood.
     *
     * Iterates through all voxels in the center chunk, and for each solid voxel,
     * checks each face direction to determine if it should be included in the mesh.
     *
     * @param neighborhood The chunk data neighborhood (center + neighbors).
     * @return std::vector<VoxelFace> Vector of visible voxel faces.
     */
    static std::vector<VoxelFace> generateFaceInstances(const ChunkDataNeighborhood& neighborhood);
};

#endif //FACECULLINGMESHER_H
