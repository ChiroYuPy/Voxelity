//
// Created by adrian on 14/05/25.
//

#include <iostream>

#include "core/Constants.h"
#include "components/ChunkMesh.h"
#include "voxelWorld/World.h"

class IChunkMesher;

Chunk::Chunk(const glm::ivec3 position)
: position(position), empty(true), state(ChunkState::UnGenerated) {
    std::cout << toString(state) << std::endl;
}

bool Chunk::isDirty() const {
    return state == ChunkState::MeshDirty;
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

ChunkMesh& Chunk::getMesh() {
    return mesh;
}

ChunkData& Chunk::getData() {
    return data;
}

void Chunk::setData(const ChunkData& newData) {
    data = newData;
    updateEmptyFlag();
}

void Chunk::setMesh(const ChunkMesh& newMesh) {
    std::cout << "Chunk " << position.x << ", " << position.y << ", " << position.z << " mesh updated" << std::endl;
    mesh.init();
    mesh.build();
    mesh = newMesh;
}

Chunk* Chunk::getNeighbor(const BlockFace direction) const {
    return neighbors[static_cast<int>(direction)];
}

void Chunk::setNeighbor(const BlockFace direction, Chunk* neighbor) {
    neighbors[static_cast<int>(direction)] = neighbor;
}

void Chunk::setState(const ChunkState newState) {
    state.store(newState, std::memory_order_release);
    std::cout << toString(newState) << std::endl;
}

ChunkState Chunk::getState() const {
    return state.load(std::memory_order_acquire);
}