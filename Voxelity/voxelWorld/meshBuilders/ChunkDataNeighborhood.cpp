//
// Created by adrian on 18/05/25.
//

#include "ChunkDataNeighborhood.h"

ChunkDataNeighborhood::ChunkDataNeighborhood()
: center(nullptr) {}

ChunkDataNeighborhood::ChunkDataNeighborhood(ChunkData* centerChunk, const std::array<ChunkData*, 6>& neighborsArray)
: center(centerChunk), neighbors(neighborsArray) {}

ChunkData* ChunkDataNeighborhood::getNeighbor(const BlockFace direction) const {
    return neighbors[static_cast<int>(direction)];
}