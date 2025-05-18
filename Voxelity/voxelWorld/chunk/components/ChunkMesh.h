//
// Created by adrian on 14/05/25.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "rendering/VoxelFace.h"
#include "math/Direction.h"

class Chunk;

struct Voxel;

class ChunkMesh {
public:
    explicit ChunkMesh();

    void init();

    ~ChunkMesh();

    void setVoxelFaces(std::vector<VoxelFace> newFaces);

    void render() const;

    [[nodiscard]] bool hasVisibleFaces() const;

    void build() const;

    void clear();

private:
    GLuint vao{}, staticVBO{}, vbo{};
    std::vector<VoxelFace> voxelFaces;
};

#endif //MESH_H
