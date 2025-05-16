//
// Created by adrian on 14/05/25.
//

#ifndef VOXELFACE_H
#define VOXELFACE_H

#include <GLT.h>

// Number of bits allocated for each attribute
#define BITS_POS     4    // 4 bits -> 16 possible values (e.g., chunk size 16)
#define BITS_FACEID  3    // 3 bits -> 8 possible values (6 used for voxel faces)
#define BITS_TYPE    8    // 8 bits -> 255 possible values (block type)
// Total 32 bits -> 32 - (4 + 4 + 4 + 3 + 8) = 9 bits free (for future use)

class VoxelFace {
public:
    static constexpr int SHIFT_X = 0;
    static constexpr int SHIFT_Y = SHIFT_X + BITS_POS;
    static constexpr int SHIFT_Z = SHIFT_Y + BITS_POS;
    static constexpr int SHIFT_FACE = SHIFT_Z + BITS_POS;
    static constexpr int SHIFT_TYPE = SHIFT_FACE + BITS_FACEID;

    uint32_t data;

    VoxelFace(const glm::ivec3 pos, const int faceId, const int textureID) {
        data = (static_cast<uint32_t>(pos.x) << SHIFT_X)
             | (static_cast<uint32_t>(pos.y) << SHIFT_Y)
             | (static_cast<uint32_t>(pos.z) << SHIFT_Z)
             | (static_cast<uint32_t>(faceId) << SHIFT_FACE)
             | (static_cast<uint32_t>(textureID) << SHIFT_TYPE);
    }
};

#endif //VOXELFACE_H
