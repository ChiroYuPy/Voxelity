//
// Created by adrian on 14/05/25.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "VoxelFace.h"
#include "voxelWorld/Chunk.h"

struct Voxel;

class ChunkMesh {
public:
    explicit ChunkMesh(Chunk* chunk);
    ~ChunkMesh();

    void render() const;

    [[nodiscard]] bool hasVisibleFaces() const;

    static bool isFaceVisible(int x, int y, int z, const Chunk &chunk, Direction direction);

    static std::vector<VoxelFace> generateFaceInstances(const Chunk &chunk) ;

    void build(const Chunk* chunk);

    void clear();

private:
    Chunk* chunk;
    GLuint vao{}, staticVBO, vbo{};
    std::vector<VoxelFace> voxelFaces;
};

#endif //MESH_H
