//
// Created by adrian on 16/05/25.
//

#include "VoxelStorage.h"

const Voxel& VoxelStorage::get(const unsigned int x, const unsigned int y, const unsigned int z) const {
    return voxels[index(x, y, z)];
}

Voxel& VoxelStorage::get(const unsigned int x, const unsigned int y, const unsigned int z) {
    return voxels[index(x, y, z)];
}

void VoxelStorage::set(const unsigned int x, const unsigned int y, const unsigned int z, const Voxel voxel) {
    voxels[index(x,y,z)] = voxel;
}

const Voxel& VoxelStorage::operator()(const unsigned int x, const unsigned int y, const unsigned int z) const {
    return get(x, y, z);
}

Voxel& VoxelStorage::operator()(const unsigned int x, const unsigned int y, const unsigned int z) {
    return get(x, y, z);
}

[[nodiscard]] const std::array<Voxel, Constants::CHUNK_VOLUME>& VoxelStorage::getVoxels() const {
    return voxels;
}

void VoxelStorage::fill(const BlockType type) {
    for (auto& voxel : voxels) {
        voxel.setType(type);
    }
}

void VoxelStorage::clear() {
    fill(BlockType::Air);
}

size_t VoxelStorage::index(const unsigned int x, const unsigned int y, const unsigned int z) {
    return x + (y * Constants::CHUNK_SIZE) + (z * Constants::CHUNK_AREA);
}