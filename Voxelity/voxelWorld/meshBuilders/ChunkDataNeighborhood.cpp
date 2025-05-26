//
// Created by adrian on 18/05/25.
//

#include "ChunkDataNeighborhood.h"

ChunkDataNeighborhood::ChunkDataNeighborhood()
: center(nullptr) {}

ChunkDataNeighborhood::ChunkDataNeighborhood(VoxelStorage* centerChunk, const std::array<VoxelStorage*, 6>& neighborsArray)
: center(centerChunk), neighbors(neighborsArray) {}

VoxelStorage* ChunkDataNeighborhood::getNeighbor(const BlockFace direction) const {
    return neighbors[static_cast<int>(direction)];
}