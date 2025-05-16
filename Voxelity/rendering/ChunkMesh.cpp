//
// Created by adrian on 14/05/25.
//

#include "rendering/ChunkMesh.h"

#include <iostream>

#include "core/Constants.h"
#include "voxelWorld/Chunk.h"

ChunkMesh::ChunkMesh(Chunk* chunk) : chunk(chunk) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(VoxelFace), reinterpret_cast<void *>(offsetof(VoxelFace, data)));
    glVertexAttribDivisor(0, 1);
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void ChunkMesh::render() const {
    if (voxelFaces.empty()) return;

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(voxelFaces.size()));
}

bool ChunkMesh::hasVisibleFaces() const {
    return !voxelFaces.empty();
}

bool ChunkMesh::isFaceVisible(const int x, const int y, const int z, const Chunk& chunk, const Direction direction) {
    const glm::ivec3 offset = DIRECTION_NORMALS[static_cast<int>(direction)];
    glm::ivec3 neighborPos = { x + offset.x, y + offset.y, z + offset.z };

    const Chunk* targetChunk = &chunk;

    if (neighborPos.x < 0 || neighborPos.x >= Constants::ChunkSize ||
        neighborPos.y < 0 || neighborPos.y >= Constants::ChunkSize ||
        neighborPos.z < 0 || neighborPos.z >= Constants::ChunkSize) {

        targetChunk = chunk.getNeighbor(direction);
        if (!targetChunk) return true;

        neighborPos.x = (neighborPos.x + Constants::ChunkSize) % Constants::ChunkSize;
        neighborPos.y = (neighborPos.y + Constants::ChunkSize) % Constants::ChunkSize;
        neighborPos.z = (neighborPos.z + Constants::ChunkSize) % Constants::ChunkSize;
        }

    const Voxel voxel = targetChunk->getData().get(neighborPos.x, neighborPos.y, neighborPos.z);
    return !voxel.isSolid();
}

std::vector<VoxelFace> ChunkMesh::generateFaceInstances(const Chunk& chunk) {
    std::vector<VoxelFace> faces;

    for (int z = 0; z < Constants::ChunkSize; ++z)
        for (int y = 0; y < Constants::ChunkSize; ++y)
            for (int x = 0; x < Constants::ChunkSize; ++x) {
                const Voxel* voxel = &chunk.getData().get(x, y, z);
                if (!voxel->isSolid()) continue;

                for (const Direction direction : DIRECTIONS) {
                    if (isFaceVisible(x, y, z, chunk, direction)) {
                        faces.emplace_back(glm::ivec3(x, y, z), static_cast<int>(direction), static_cast<int>(voxel->getType()));
                    }
                }
            }

    return faces;
}

void ChunkMesh::build(const Chunk* chunk) {
    voxelFaces = generateFaceInstances(*chunk);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxelFaces.size() * sizeof(VoxelFace)), voxelFaces.data(), GL_STATIC_DRAW);
}

void ChunkMesh::clear() {
    voxelFaces.clear();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}
