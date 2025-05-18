//
// Created by adrian on 14/05/25.
//

#include "ChunkMesh.h"

#include <iostream>

#include "ChunkMeshData.h"

ChunkMesh::ChunkMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(VoxelFace), reinterpret_cast<void*>(offsetof(VoxelFace, data)));
    glVertexAttribDivisor(0, 1);
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void ChunkMesh::upload(const ChunkMeshData& data) {
    const auto& faces = data.getVoxelFaces();
    instanceCount = static_cast<GLsizei>(faces.size());

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(instanceCount * sizeof(VoxelFace)), faces.data(), GL_STATIC_DRAW);
}

void ChunkMesh::render() const {
    if (instanceCount == 0) return;

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);
}

void ChunkMesh::clear() {
    instanceCount = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}
