//
// Created by adrian on 18/05/25.
//

#ifndef ICHUNKMESHER_H
#define ICHUNKMESHER_H

#include "../chunk/ChunkMesh.h"

class Chunk;
struct ChunkData;
class VoxelFace;

class IChunkMeshBuilder {
public:
    virtual ~IChunkMeshBuilder() = default;

    virtual std::vector<VoxelFace> mesh(Chunk&) = 0;
};

#endif //ICHUNKMESHER_H
