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

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    // Layouts
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 3, GL_INT, sizeof(VoxelFace), reinterpret_cast<void *>(offsetof(VoxelFace, pos)));
    glVertexAttribDivisor(0, 1);

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(VoxelFace), reinterpret_cast<void *>(offsetof(VoxelFace, faceId)));
    glVertexAttribDivisor(1, 1);

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

bool ChunkMesh::isFaceVisible(int x, int y, int z, const Chunk& chunk, Direction direction) {
    const glm::ivec3 offset = DIRECTION_NORMALS[static_cast<int>(direction)];
    glm::ivec3 neighborPos = { x + offset.x, y + offset.y, z + offset.z };

    const Chunk* targetChunk = &chunk;

    if (neighborPos.x < 0 || neighborPos.x >= Chunk::SIZE ||
        neighborPos.y < 0 || neighborPos.y >= Chunk::SIZE ||
        neighborPos.z < 0 || neighborPos.z >= Chunk::SIZE) {

        targetChunk = chunk.getNeighbor(direction);
        if (!targetChunk) return true;

        neighborPos.x = (neighborPos.x + Chunk::SIZE) % Chunk::SIZE;
        neighborPos.y = (neighborPos.y + Chunk::SIZE) % Chunk::SIZE;
        neighborPos.z = (neighborPos.z + Chunk::SIZE) % Chunk::SIZE;
        }

    const Voxel* voxel = targetChunk->at(neighborPos.x, neighborPos.y, neighborPos.z);
    return !voxel || !voxel->isSolid();
}

std::vector<VoxelFace> ChunkMesh::generateFaceInstances(const Chunk& chunk) {
    std::vector<VoxelFace> faces;

    for (int z = 0; z < Chunk::SIZE; ++z)
        for (int y = 0; y < Chunk::SIZE; ++y)
            for (int x = 0; x < Chunk::SIZE; ++x) {
                const Voxel* voxel = chunk.at(x, y, z);
                if (!voxel->isSolid()) continue;

                const glm::ivec3 worldPos = chunk.getPosition() * Chunk::SIZE + glm::ivec3(x, y, z);
                for (const Direction direction : DIRECTIONS) {
                    if (isFaceVisible(x, y, z, chunk, direction)) {
                        faces.emplace_back(worldPos, static_cast<int>(direction), static_cast<int>(voxel->type));
                    }
                }
            }

    return faces;
}

void ChunkMesh::build(const Chunk* chunk) {
    voxelFaces = generateFaceInstances(*chunk);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, voxelFaces.size() * sizeof(VoxelFace), voxelFaces.data(), GL_STATIC_DRAW);
}