//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/Chunk.h"

#include "rendering/ChunkMesh.h"
#include "voxelWorld/World.h"

Chunk::Chunk(const int cx, const int cy, const int cz, const World* world) : world(world), dirty(true), position{cx, cy, cz} {
    mesh = std::make_shared<ChunkMesh>();
}

void Chunk::setDirty(const bool dirty_) {
    dirty = dirty_;
}

bool Chunk::isDirty() const {
    return dirty;
}

const Voxel* Chunk::getVoxelAt(const int x, const int y, const int z) const {
    return &voxels[index(x, y, z)];
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

std::shared_ptr<ChunkMesh> Chunk::getMesh() const {
    return mesh;
}

void Chunk::updateMesh() {
    if (isDirty()) buildMesh();
    setDirty(false);
}

Chunk* Chunk::getNeighbor(const uint8_t direction) const {
    return neighbors[static_cast<int>(direction)];
}

void Chunk::setNeighbor(const uint8_t direction, Chunk* neighbor) {
    neighbors[static_cast<int>(direction)] = neighbor;
}

void Chunk::buildMesh() const {
    mesh->build(this, world);
}
