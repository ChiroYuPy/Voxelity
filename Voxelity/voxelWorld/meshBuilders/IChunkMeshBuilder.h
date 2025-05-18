//
// Created by adrian on 18/05/25.
//

#ifndef ICHUNKMESHER_H
#define ICHUNKMESHER_H

#include "../chunk/ChunkMesh.h"

class Chunk;
class VoxelFace;

#include <array>

#include "voxelWorld/chunk/ChunkData.h"

#include "math/BlockFace.h"

struct ChunkDataNeighborhood {
    ChunkDataNeighborhood()
    : center(nullptr) {}

    ChunkDataNeighborhood(ChunkData* centerChunk, const std::array<ChunkData*, 6>& neighborsArray)
    : center(centerChunk), neighbors(neighborsArray) {}

    [[nodiscard]] ChunkData* getCenter() const {
        return center;
    }

    [[nodiscard]] ChunkData* getNeighbor(const BlockFace direction) const {
        return neighbors[static_cast<int>(direction)];
    }

private:
    ChunkData* center;
    std::array<ChunkData*, 6> neighbors{};
};

class IChunkMeshBuilder {
public:
    virtual ~IChunkMeshBuilder() = default;

    virtual std::vector<VoxelFace> mesh(const ChunkDataNeighborhood& neighborhood) = 0;
};

#endif //ICHUNKMESHER_H
