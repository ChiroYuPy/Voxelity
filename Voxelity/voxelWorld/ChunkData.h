//
// Created by adrian on 16/05/25.
//

#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <cstdint>
#include <vector>

struct ChunkData {
    int x, y, z;
    std::vector<uint8_t> voxels;
};

#endif //CHUNKDATA_H
