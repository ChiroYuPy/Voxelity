//
// Created by adrian on 14/05/25.
//

#include "rendering/ChunkMesh.h"

#include <iostream>

#include "voxelWorld/World.h"

ChunkMesh::ChunkMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Allocation vide au début (on remplit avec update)
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 3, GL_INT, sizeof(VoxelFace), static_cast<void *>(nullptr));
    glVertexAttribDivisor(0, 1);

    // faceId
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(VoxelFace), reinterpret_cast<void *>(offsetof(VoxelFace, faceId)));
    glVertexAttribDivisor(1, 1);

    // type
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(VoxelFace), reinterpret_cast<void *>(offsetof(VoxelFace, type)));
    glVertexAttribDivisor(2, 1);
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void ChunkMesh::render() const {
    if (voxelFaces.empty()) return;

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, voxelFaces.size());
}

bool ChunkMesh::isFaceVisible(const int x, const int y, const int z, const Chunk& chunk, const World& world, const int face) {
    static const glm::ivec3 directions[6] = {
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0, -1, 0},
        {0, 0, 1}, {0, 0, -1}
    };
    const glm::ivec3 offset = directions[face];
    const int nx = x + offset.x;
    const int ny = y + offset.y;
    const int nz = z + offset.z;

    if (nx < 0 || ny < 0 || nz < 0 || nx >= CHUNK_SIZE || ny >= CHUNK_SIZE || nz >= CHUNK_SIZE) {
        const glm::ivec3 worldPos = chunk.getPosition() * CHUNK_SIZE + glm::ivec3(nx, ny, nz);
        const Voxel *voxel = world.getVoxelAt(worldPos.x, worldPos.y, worldPos.z);
        if (voxel) return !voxel->isSolid();
        return false;
    }
    return !chunk.getVoxelAt(nx, ny, nz)->isSolid();
}

std::vector<VoxelFace> ChunkMesh::generateFaceInstances(const Chunk& chunk, const World& world) {
    std::vector<VoxelFace> faces;

    for (int z = 0; z < CHUNK_SIZE; ++z)
        for (int y = 0; y < CHUNK_SIZE; ++y)
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                const Voxel* voxel = chunk.getVoxelAt(x, y, z);
                if (!voxel->isSolid()) continue;

                const glm::ivec3 worldPos = chunk.getPosition() * CHUNK_SIZE + glm::ivec3(x, y, z);
                for (int face = 0; face < 6; ++face) {
                    if (isFaceVisible(x, y, z, chunk, world, face)) {
                        faces.emplace_back(worldPos, face, static_cast<int>(voxel->type));
                    }
                }
            }

    return faces;
}

void ChunkMesh::build(const Chunk* chunk, const World* world) {
    voxelFaces = generateFaceInstances(*chunk, *world);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, voxelFaces.size() * sizeof(VoxelFace), voxelFaces.data(), GL_STATIC_DRAW);
}