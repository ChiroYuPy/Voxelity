//
// Created by adrian on 14/05/25.
//

#ifndef VOXELFACE_H
#define VOXELFACE_H

class VoxelFace {
public:
    glm::ivec3 pos;
    int faceId;
    int type;

    VoxelFace(const glm::ivec3 p, const int id, const int type) : pos(p), faceId(id), type(type) {}
};

#endif //VOXELFACE_H
