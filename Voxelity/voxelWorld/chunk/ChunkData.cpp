//
// Created by adrian on 16/05/25.
//

#include "ChunkData.h"

const Voxel& ChunkData::get(const unsigned int x, const unsigned int y, const unsigned int z) const {
    return voxels[index(x, y, z)];
}

Voxel& ChunkData::get(const unsigned int x, const unsigned int y, const unsigned int z) {
    return voxels[index(x, y, z)];
}

void ChunkData::set(const unsigned int x, const unsigned int y, const unsigned int z, const Voxel voxel) {
    voxels[index(x,y,z)] = voxel;
}

const Voxel& ChunkData::operator()(const unsigned int x, const unsigned int y, const unsigned int z) const {
    return get(x, y, z);
}

Voxel& ChunkData::operator()(const unsigned int x, const unsigned int y, const unsigned int z) {
    return get(x, y, z);
}

[[nodiscard]] const std::array<Voxel, Constants::ChunkVolume>& ChunkData::getVoxels() const {
    return voxels;
}

size_t ChunkData::index(const unsigned int x, const unsigned int y, const unsigned int z) {
    return x + (y * Constants::ChunkSize) + (z * Constants::ChunkArea);
}