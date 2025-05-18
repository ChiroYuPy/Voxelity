//
// Created by adrian on 14/05/25.
//

#include "core/Constants.h"
#include "ChunkMesh.h"
#include "voxelWorld/World.h"

class IChunkMeshBuilder;

Chunk::Chunk(const glm::ivec3 position)
: position(position), empty(true), state(ChunkState::UnGenerated), neighbors() {}

bool Chunk::isDirty() const {
    return state == ChunkState::MeshDirty;
}

bool Chunk::isEmpty() const {
    return empty;
}

void Chunk::render() const {
    mesh.render();
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

ChunkData& Chunk::getData() {
    return data;
}

const ChunkData& Chunk::getData() const {
    return data;
}

ChunkMesh& Chunk::getMesh() {
    return mesh;
}

const ChunkMesh& Chunk::getMesh() const {
    return mesh;
}

void Chunk::setData(const ChunkData& newData) {
    data = newData;
    updateEmptyFlag();
}

void Chunk::setMeshData(const ChunkMeshData& newMeshData) {
    mesh.upload(newMeshData);
}

Chunk* Chunk::getNeighbor(const BlockFace direction) const {
    return neighbors[static_cast<int>(direction)];
}

void Chunk::setNeighbor(const BlockFace direction, Chunk* neighbor) {
    neighbors[static_cast<int>(direction)] = neighbor;
}

std::array<Chunk*, 6> Chunk::getNeighbors() const {
    return neighbors;
}

void Chunk::setState(const ChunkState newState) {
    state.store(newState, std::memory_order_release);
}

ChunkState Chunk::getState() const {
    return state.load(std::memory_order_acquire);
}