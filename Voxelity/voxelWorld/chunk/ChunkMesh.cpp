//
// Created by adrian on 14/05/25.
//

#include "ChunkMesh.h"

#include <iostream>

ChunkMesh::ChunkMesh() {
    init();
}

void ChunkMesh::init() {
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

void ChunkMesh::setVoxelFaces(std::vector<VoxelFace> newFaces) {
    voxelFaces = std::move(newFaces);
}

void ChunkMesh::render() const {
    if (voxelFaces.empty()) return;

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(voxelFaces.size()));
}

bool ChunkMesh::hasVisibleFaces() const {
    return !voxelFaces.empty();
}

void ChunkMesh::build() const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(voxelFaces.size() * sizeof(VoxelFace)), voxelFaces.data(), GL_STATIC_DRAW);
}

void ChunkMesh::clear() {
    voxelFaces.clear();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glBindVertexArray(vao);
}