//
// Created by adrian on 14/05/25.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "VoxelFace.h"
#include "math/BlockFace.h"

class ChunkMeshData;
class Chunk;

struct Voxel;

class ChunkMesh {
public:
    explicit ChunkMesh();
    ~ChunkMesh();

    void upload(const ChunkMeshData& data);
    void render() const;
    void clear();

private:
    GLuint vao{}, vbo{};
    GLsizei instanceCount{};
};

#endif //MESH_H
