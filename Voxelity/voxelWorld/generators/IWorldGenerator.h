//
// Created by adrian on 14/05/25.
//

#ifndef IWORLDGENERATOR_H
#define IWORLDGENERATOR_H

#include "blocks/BlockType.h"

struct VoxelStorage;
class Chunk;

/**
 * @brief Abstract interface for chunk voxel data generators.
 *
 * Classes implementing this interface provide methods to generate voxel
 * types for individual positions and to generate a full chunk's data.
 */
class IChunkGenerator {
public:
    virtual ~IChunkGenerator() = default;

    /**
    * @brief Generate the voxel type at a given global position.
    *
    * This pure virtual method must be implemented to define how to
    * determine the BlockType of a voxel at the specified global coordinates.
    *
    * @param globalPos The global 3D position in world coordinates.
    * @return The BlockType for the voxel at the given position.
    */
    virtual BlockType generateVoxel(const glm::ivec3& globalPos) = 0;

    /**
    * @brief Generate the full chunk data at the specified global chunk position.
    *
    * This method generates a complete ChunkData object by invoking generateVoxel
    * for each voxel within the chunk bounds.
    *
    * @param globalChunkPos The global chunk coordinates identifying which chunk to generate.
    * @return A ChunkData instance containing all voxel data for the chunk.
    */
    VoxelStorage generate(glm::ivec3 globalChunkPos);
};

#endif //IWORLDGENERATOR_H
