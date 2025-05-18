//
// Created by adrian on 18/05/25.
//

#ifndef ICHUNKMESHER_H
#define ICHUNKMESHER_H

#include "voxelWorld/chunk/components/ChunkMesh.h"

class Chunk;
struct ChunkData;
class VoxelFace;

class IChunkMesher {
public:
    virtual ~IChunkMesher() = default;

    virtual std::vector<VoxelFace> mesh(Chunk&) = 0;
};

#endif //ICHUNKMESHER_H
