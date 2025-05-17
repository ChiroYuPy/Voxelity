//
// Created by adrian on 14/05/25.
//

#include "core/Constants.h"
#include "rendering/ChunkMesh.h"
#include "voxelWorld/World.h"

Chunk::Chunk(const glm::ivec3 position)
: position(position), mesh(this), dirty(true), empty(true), state(ChunkState::Unloaded) {}

void Chunk::markDirty() {
    dirty = true;
}

void Chunk::unmarkDirty() {
    dirty = false;
}

bool Chunk::isDirty() const {
    return dirty;
}

bool Chunk::isEmpty() const {
    return empty;
}

void Chunk::updateEmptyFlag() {
    for (const Voxel& voxel : data.getVoxels()) {
        if (voxel.getType() != BlockType::Air) {
            empty = false;
            return;
        }
    }
    empty = true;
}

glm::ivec3 Chunk::getPosition() const {
    return position;
}

glm::ivec3 Chunk::getWorldPosition() const {
    return position * Constants::ChunkSize;
}

const ChunkMesh& Chunk::getMesh() const {
    return mesh;
}

const ChunkData& Chunk::getData() const {
    return data;
}

void Chunk::setData(const ChunkData& newData) {
    data = newData;
    markDirty();
    updateEmptyFlag();
}

void Chunk::updateMesh() {
    if (isDirty()) {
        buildMesh();
        updateEmptyFlag();
        unmarkDirty();
    }
}

void Chunk::buildMesh() {
    mesh.build(this);
}

Chunk* Chunk::getNeighbor(const Direction direction) const {
    return neighbors[static_cast<int>(direction)];
}

void Chunk::setNeighbor(const Direction direction, Chunk* neighbor) {
    neighbors[static_cast<int>(direction)] = neighbor;
}

void Chunk::setState(const ChunkState s) {
    std::lock_guard<std::mutex> lock(mutex);
    state = s;
}

ChunkState Chunk::getState() const {
    return state;
}