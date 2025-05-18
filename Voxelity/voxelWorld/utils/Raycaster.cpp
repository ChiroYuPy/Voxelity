//
// Created by adrian on 18/05/25.
//

#include "Raycaster.h"

#include "../WorldChunkData.h"

std::optional<RaycastHit> Raycaster::castRay(const glm::vec3& origin, const glm::vec3& direction,
                                             float maxDistance, const WorldChunkData& world) {

    glm::ivec3 originPos = glm::floor(origin);
    glm::vec3 directionPos = glm::normalize(direction);

    glm::vec3 deltaDist;
    deltaDist.x = (directionPos.x == 0.0f) ? 1e30f : std::abs(1.0f / directionPos.x);
    deltaDist.y = (directionPos.y == 0.0f) ? 1e30f : std::abs(1.0f / directionPos.y);
    deltaDist.z = (directionPos.z == 0.0f) ? 1e30f : std::abs(1.0f / directionPos.z);

    glm::ivec3 step;
    step.x = (directionPos.x < 0) ? -1 : 1;
    step.y = (directionPos.y < 0) ? -1 : 1;
    step.z = (directionPos.z < 0) ? -1 : 1;

    glm::vec3 sideDist;

    if (directionPos.x < 0) sideDist.x = (origin.x - static_cast<float>(originPos.x)) * deltaDist.x;
    else sideDist.x = (static_cast<float>(originPos.x) + 1.0f - origin.x) * deltaDist.x;

    if (directionPos.y < 0) sideDist.y = (origin.y - static_cast<float>(originPos.y)) * deltaDist.y;
    else sideDist.y = (static_cast<float>(originPos.y) + 1.0f - origin.y) * deltaDist.y;

    if (directionPos.z < 0) sideDist.z = (origin.z - static_cast<float>(originPos.z)) * deltaDist.z;
    else sideDist.z = (static_cast<float>(originPos.z) + 1.0f - origin.z) * deltaDist.z;

    float distTravelled = 0.0f;

    while (distTravelled <= maxDistance) {
        if (world.hasBlockAt(originPos)) {
            BlockFace face;

            if (sideDist.x - deltaDist.x < sideDist.y - deltaDist.y &&
                sideDist.x - deltaDist.x < sideDist.z - deltaDist.z) {
                face = (step.x > 0) ? XM : XP;
            } else if (sideDist.y - deltaDist.y < sideDist.z - deltaDist.z) {
                face = (step.y > 0) ? YM : YP;
            } else {
                face = (step.z > 0) ? ZM : ZP;
            }

            return RaycastHit{originPos, face};
        }

        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                originPos.x += step.x;
                distTravelled = sideDist.x;
                sideDist.x += deltaDist.x;
            } else {
                originPos.z += step.z;
                distTravelled = sideDist.z;
                sideDist.z += deltaDist.z;
            }
        } else {
            if (sideDist.y < sideDist.z) {
                originPos.y += step.y;
                distTravelled = sideDist.y;
                sideDist.y += deltaDist.y;
            } else {
                originPos.z += step.z;
                distTravelled = sideDist.z;
                sideDist.z += deltaDist.z;
            }
        }
    }

    return std::nullopt;
}