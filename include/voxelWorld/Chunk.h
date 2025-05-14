//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <GLT.h>

#include <array>

#include "Voxel.h"

constexpr int CHUNK_SIZE = 16;

class Chunk {
public:
    Chunk(int cx, int cy, int cz);

    Voxel get(int x, int y, int z) const;
    void set(int x, int y, int z, Voxel voxel);

    void set(int x, int y, int z, BlockType blockType);

    glm::ivec3 getPosition() const;

private:
    static int index(int x, int y, int z);

    glm::ivec3 position;
    std::array<Voxel, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels;
};

#endif //CHUNK_H
