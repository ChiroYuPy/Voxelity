//
// Created by adrian on 18/05/25.
//

#include "Raycaster.h"

#include "voxelWorld/chunk/ChunkManager.h"

std::optional<RaycastHit> Raycaster::castRay(const glm::vec3& origin, const glm::vec3& direction,
                                             float maxDistance, const ChunkManager& world) {
    glm::vec3 dir = glm::normalize(direction);

    // Position dans la grille voxel (int)
    glm::ivec3 pos = glm::floor(origin);

    // Distance que le rayon doit parcourir pour traverser une case sur chaque axe
    glm::vec3 deltaDist;
    deltaDist.x = (dir.x == 0.0f) ? 1e30f : std::abs(1.0f / dir.x);
    deltaDist.y = (dir.y == 0.0f) ? 1e30f : std::abs(1.0f / dir.y);
    deltaDist.z = (dir.z == 0.0f) ? 1e30f : std::abs(1.0f / dir.z);

    // Étape (1 ou -1) selon le signe de la direction
    glm::ivec3 step;
    step.x = (dir.x < 0) ? -1 : 1;
    step.y = (dir.y < 0) ? -1 : 1;
    step.z = (dir.z < 0) ? -1 : 1;

    // Distance jusqu’au prochain bord de voxel
    glm::vec3 sideDist;

    // Calcul initial du sideDist sur chaque axe
    if (dir.x < 0) sideDist.x = (origin.x - static_cast<float>(pos.x)) * deltaDist.x;
    else sideDist.x = (static_cast<float>(pos.x) + 1.0f - origin.x) * deltaDist.x;

    if (dir.y < 0) sideDist.y = (origin.y - static_cast<float>(pos.y)) * deltaDist.y;
    else sideDist.y = (static_cast<float>(pos.y) + 1.0f - origin.y) * deltaDist.y;

    if (dir.z < 0) sideDist.z = (origin.z - static_cast<float>(pos.z)) * deltaDist.z;
    else sideDist.z = (static_cast<float>(pos.z) + 1.0f - origin.z) * deltaDist.z;

    float distTravelled = 0.0f;

    while (distTravelled <= maxDistance) {
        if (world.hasBlockAt(pos)) {
            BlockFace face;

            if (sideDist.x - deltaDist.x < sideDist.y - deltaDist.y &&
                sideDist.x - deltaDist.x < sideDist.z - deltaDist.z) {
                face = (step.x > 0) ? XM : XP;
            } else if (sideDist.y - deltaDist.y < sideDist.z - deltaDist.z) {
                face = (step.y > 0) ? YM : YP;
            } else {
                face = (step.z > 0) ? ZM : ZP;
            }

            return RaycastHit{pos, face};
        }

        // Avancer dans la grille : trouver le plus petit sideDist
        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                pos.x += step.x;
                distTravelled = sideDist.x;
                sideDist.x += deltaDist.x;
            } else {
                pos.z += step.z;
                distTravelled = sideDist.z;
                sideDist.z += deltaDist.z;
            }
        } else {
            if (sideDist.y < sideDist.z) {
                pos.y += step.y;
                distTravelled = sideDist.y;
                sideDist.y += deltaDist.y;
            } else {
                pos.z += step.z;
                distTravelled = sideDist.z;
                sideDist.z += deltaDist.z;
            }
        }
    }

    return std::nullopt;
}