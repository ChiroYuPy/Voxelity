//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/Chunk.h"

#include <iostream>

#include "rendering/ChunkMesh.h"
#include "voxelWorld/World.h"

Chunk::Chunk(const glm::ivec3 position) : position(position), dirty(true), empty(true) {
    mesh = std::make_shared<ChunkMesh>(this);
    fill(BlockType::Air);
}

void Chunk::markDirty() {
    dirty = true;
}

bool Chunk::isDirty() const {
    return dirty;
}

bool Chunk::isEmpty() const {
    return empty;
}

void Chunk::fill(BlockType blockType) {
    const Voxel voxelToSet{static_cast<uint8_t>(blockType)};
    for (auto& voxel : voxels) {
        voxel = voxelToSet;
    }
    empty = blockType == BlockType::Air;
}

void Chunk::updateEmptyFlag() {
    for (const Voxel& voxel : voxels) {
        if (voxel.type != BlockType::Air) {
            empty = false;
            return;
        }
    }
    empty = true;
}

const Voxel* Chunk::at(const unsigned int x, const unsigned int y, const unsigned int z) const {
    return &voxels[index(x, y, z)];
}

void Chunk::set(const unsigned int x, unsigned const int y, unsigned const int z, const BlockType blockType) {
    voxels[index(x, y, z)] = Voxel{static_cast<uint8_t>(blockType)};
    markDirty();
}

const Voxel* Chunk::at(const glm::uvec3 position) const {
    return &voxels[index(position)];
}

void Chunk::set(const glm::uvec3 position, const BlockType blockType) {
    voxels[index(position)] = Voxel{static_cast<uint8_t>(blockType)};
    markDirty();
}

glm::ivec3 Chunk::getPosition() const {
    return position;
}

unsigned int Chunk::index(const unsigned int x, const unsigned int y, const unsigned int z) {
    return x + SIZE * (y + SIZE * z);
}

unsigned int Chunk::index(const glm::uvec3 position) {
    return position.x + SIZE * position.y + SIZE * position.z;
}

std::shared_ptr<ChunkMesh> Chunk::getMesh() const {
    return mesh;
}

void Chunk::updateMesh() {
    if (isDirty()) {
        buildMesh();
        updateEmptyFlag();
        dirty = false;
    }
}

void Chunk::buildMesh() const {
    mesh->build(this);
}

Chunk* Chunk::getNeighbor(const Direction direction) const {
    return neighbors[static_cast<int>(direction)];
}

void Chunk::setNeighbor(const Direction direction, Chunk* neighbor) {
    neighbors[static_cast<int>(direction)] = neighbor;
}
