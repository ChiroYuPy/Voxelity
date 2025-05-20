//
// Created by adrian on 18/05/25.
//

#include "Raycaster.h"

#include "../WorldChunkData.h"

#include "Raycaster.h"
#include "../WorldChunkData.h"

std::optional<RaycastHit> Raycaster::castRay(const glm::vec3& origin, const glm::vec3& direction,
                                             float maxDistance, const WorldChunkData& world) {
    glm::vec3 dir = glm::normalize(direction);
    glm::ivec3 pos = glm::floor(origin);

    glm::ivec3 step{
        (dir.x < 0) ? -1 : 1,
        (dir.y < 0) ? -1 : 1,
        (dir.z < 0) ? -1 : 1
    };

    glm::vec3 delta{
        (dir.x != 0.0f) ? std::abs(1.0f / dir.x) : 1e30f,
        (dir.y != 0.0f) ? std::abs(1.0f / dir.y) : 1e30f,
        (dir.z != 0.0f) ? std::abs(1.0f / dir.z) : 1e30f
    };

    glm::vec3 sideDist;
    sideDist.x = (step.x > 0) ? (glm::floor(origin.x + 1.0f) - origin.x) * delta.x
                              : (origin.x - glm::floor(origin.x)) * delta.x;
    sideDist.y = (step.y > 0) ? (glm::floor(origin.y + 1.0f) - origin.y) * delta.y
                              : (origin.y - glm::floor(origin.y)) * delta.y;
    sideDist.z = (step.z > 0) ? (glm::floor(origin.z + 1.0f) - origin.z) * delta.z
                              : (origin.z - glm::floor(origin.z)) * delta.z;

    float dist = 0.0f;

    while (dist <= maxDistance) {
        if (world.hasBlockAt(pos)) {
            return RaycastHit{pos, BlockFace::Unknown}; // dev fallback
        }

        // Détermination de l'axe minimum = face touchée
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            pos.x += step.x;
            dist = sideDist.x;
            sideDist.x += delta.x;
            if (world.hasBlockAt(pos))
                return RaycastHit{pos, (step.x > 0) ? BlockFace::XM : BlockFace::XP};
        } else if (sideDist.y < sideDist.z) {
            pos.y += step.y;
            dist = sideDist.y;
            sideDist.y += delta.y;
            if (world.hasBlockAt(pos))
                return RaycastHit{pos, (step.y > 0) ? BlockFace::YM : BlockFace::YP};
        } else {
            pos.z += step.z;
            dist = sideDist.z;
            sideDist.z += delta.z;
            if (world.hasBlockAt(pos))
                return RaycastHit{pos, (step.z > 0) ? BlockFace::ZM : BlockFace::ZP};
        }
    }

    return std::nullopt;
}
