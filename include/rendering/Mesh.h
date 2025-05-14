//
// Created by adrian on 14/05/25.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "VoxelFace.h"
#include "voxelWorld/Chunk.h"

struct Voxel;

class Mesh {
public:
    explicit Mesh();
    ~Mesh();

    void render() const;

    static bool isFaceVisible(int x, int y, int z, const Chunk &chunk, const World& world, int face);

    static std::vector<VoxelFace> generateFaceInstances(const Chunk &chunk, const World& world) ;

    void build(const Chunk* chunk, const World* world);

private:
    GLuint vao{}, vbo{};
    std::vector<VoxelFace> voxelFaces;
};

#endif //MESH_H
