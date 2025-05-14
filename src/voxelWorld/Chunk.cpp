//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/Chunk.h"

Chunk::Chunk(const int cx, const int cy, const int cz) : position{cx, cy, cz}, voxels() {}

Voxel Chunk::get(const int x, const int y, const int z) const {
    return voxels[index(x, y, z)];
}

void Chunk::set(const int x, const int y, const int z, const Voxel voxel) {
    voxels[index(x, y, z)] = voxel;
}

void Chunk::set(const int x, const int y, const int z, const BlockType blockType) {
    voxels[index(x, y, z)] = Voxel{static_cast<uint8_t>(blockType)};
}

glm::ivec3 Chunk::getPosition() const {
    return position;
}

int Chunk::index(const int x, const int y, const int z) {
    return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}