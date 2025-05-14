//
// Created by adrian on 14/05/25.
//

#ifndef VOXELFACE_H
#define VOXELFACE_H

#include <GLT.h>

class VoxelFace {
public:
    glm::ivec3 pos;
    int faceId;
    int type;

    VoxelFace(const glm::ivec3 pos, const int faceId, const int type)
        : pos(pos), faceId(faceId), type(type) {}
};

#endif //VOXELFACE_H
