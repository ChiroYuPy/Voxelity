//
// Created by adrian on 18/05/25.
//

#ifndef CHUNKDATANEIGHBORHOOD_H
#define CHUNKDATANEIGHBORHOOD_H
#include <array>

#include "math/BlockFace.h"

struct VoxelStorage;

/**
 * @brief Holds a chunk's data and its 6 neighbor chunks' data.
 *
 * Provides convenient accessors for the center chunk and its neighbors
 * based on the six possible block face directions.
 */
struct ChunkDataNeighborhood {

    /**
     * @brief Default constructor initializes center chunk to nullptr.
     */
    ChunkDataNeighborhood();

    /**
     * @brief Constructs a neighborhood from center chunk and neighbor chunks.
     *
     * @param centerChunk Pointer to the center chunk's data.
     * @param neighborsArray Array of pointers to the 6 neighbor chunks' data.
     */
    ChunkDataNeighborhood(VoxelStorage* centerChunk, const std::array<VoxelStorage*, 6>& neighborsArray);

    /**
     * @brief Get the pointer to the center chunk data.
     * @return ChunkData* Pointer to center chunk.
     */
    [[nodiscard]] VoxelStorage* getCenter() const {
        return center;
    }

    /**
     * @brief Get the pointer to a neighbor chunk data by block face direction.
     * @param direction The face direction of the neighbor chunk.
     * @return ChunkData* Pointer to the neighbor chunk or nullptr if not available.
     */
    [[nodiscard]] VoxelStorage* getNeighbor(BlockFace direction) const;

private:
    VoxelStorage* center;                       ///< Pointer to center chunk data
    std::array<VoxelStorage*, 6> neighbors{};  ///< Array of pointers to neighbor chunks in 6 directions
};

#endif //CHUNKDATANEIGHBORHOOD_H
