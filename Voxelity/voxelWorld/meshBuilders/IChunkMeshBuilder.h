//
// Created by adrian on 18/05/25.
//

#ifndef ICHUNKMESHER_H
#define ICHUNKMESHER_H

#include "../chunk/ChunkMesh.h"

struct ChunkDataNeighborhood;
class Chunk;
class VoxelFace;

/**
 * @brief Abstract interface for chunk mesh builders.
 *
 * Implementations must provide a mesh generation function
 * that produces a list of visible voxel faces for a given chunk neighborhood.
 */
class IChunkMeshBuilder {
public:
    virtual ~IChunkMeshBuilder() = default;

    /**
    * @brief Generate the mesh (list of visible voxel faces) for the chunk neighborhood.
    *
    * @param neighborhood The chunk data neighborhood including center and neighbors.
    * @return std::vector<VoxelFace> List of voxel faces that compose the mesh.
    */
    virtual std::vector<VoxelFace> mesh(const ChunkDataNeighborhood& neighborhood) = 0;
};

#endif //ICHUNKMESHER_H
