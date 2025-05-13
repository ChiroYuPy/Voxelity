//
// Created by adrian on 14/05/25.
//

#ifndef VOXELFACE_H
#define VOXELFACE_H

class VoxelFace {
public:
    glm::ivec3 pos;
    int faceId;
    glm::vec3 color;

    VoxelFace(const glm::ivec3 p, const int id, const glm::vec3 c) : pos(p), faceId(id), color(c) {}
};

#endif //VOXELFACE_H
