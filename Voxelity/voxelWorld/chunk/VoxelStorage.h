//
// Created by adrian on 16/05/25.
//

#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include "Voxel.h"
#include "core/Constants.h"

struct VoxelStorage {
    [[nodiscard]] const Voxel& get(unsigned int x, unsigned int y, unsigned int z) const;
    Voxel& get(unsigned int x, unsigned int y, unsigned int z);

    void set(unsigned int x, unsigned int y, unsigned int z, Voxel voxel);

    const Voxel& operator()(unsigned int x, unsigned int y, unsigned int z) const;
    Voxel& operator()(unsigned int x, unsigned int y, unsigned int z);

    [[nodiscard]] const std::array<Voxel, Constants::ChunkVolume>& getVoxels() const;

    void fill(BlockType type);

    void clear();

private:
    static size_t index(unsigned int x, unsigned int y, unsigned int z);

    std::array<Voxel, Constants::ChunkVolume> voxels;
};

#endif //CHUNKDATA_H
