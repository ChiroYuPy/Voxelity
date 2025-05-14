//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/Chunk.h"

#include <iostream>

#include "rendering/Mesh.h"

Chunk::Chunk(const int cx, const int cy, const int cz) : dirty(true), position{cx, cy, cz} {
    mesh = std::make_unique<Mesh>();
}

void Chunk::setDirty(const bool dirty_) {
    dirty = dirty_;
}

bool Chunk::isDirty() const {
    return dirty;
}

Voxel Chunk::get(const int x, const int y, const int z) const {
    return voxels[index(x, y, z)];
}

void Chunk::set(const int x, const int y, const int z, const Voxel voxel) {
    voxels[index(x, y, z)] = voxel;
    setDirty(true);
}

void Chunk::set(const int x, const int y, const int z, const BlockType blockType) {
    voxels[index(x, y, z)] = Voxel{static_cast<uint8_t>(blockType)};
    setDirty(true);
}

glm::ivec3 Chunk::getPosition() const {
    return position;
}

int Chunk::index(const int x, const int y, const int z) {
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

std::shared_ptr<Mesh> Chunk::getMesh() const {
    return mesh;
}

void Chunk::updateMesh() {
    if (isDirty()) buildMesh();
    setDirty(false);
}

void Chunk::buildMesh() const {
    mesh->build(this);
}
