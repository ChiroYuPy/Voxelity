//
// Created by adrian on 18/05/25.
//

#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <GLT.h>
#include <optional>

#include "math/BlockFace.h"

struct RaycastHit {
    glm::ivec3 blockPos;
    BlockFace face;
};

class WorldChunkData;

class Raycaster {
public:
    static std::optional<RaycastHit> castRay(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, const WorldChunkData& world);
};

#endif //RAYCASTER_H
