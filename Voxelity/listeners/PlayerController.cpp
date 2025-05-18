//
// Created by adrian on 18/05/25.
//

#include "PlayerController.h"

#include "voxelWorld/chunk/ChunkManager.h"
#include "voxelWorld/utils/Raycaster.h"

void PlayerController::onEvent(Event& e) {
    // if (e.type == EventType::MouseButtonPressed) {
    // TODO some mouse events...
    // }
}

void PlayerController::tryBreakBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkManager& world) {
    auto hitOpt = Raycaster::castRay(cameraPos, cameraDir, Constants::PlayerReach, world);
    if (!hitOpt) return;

    const auto&[blockPos, face] = *hitOpt;

    world.removeBlock(blockPos);
}

void PlayerController::tryPlaceBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkManager& world, const BlockType blockType) {
    auto hitOpt = Raycaster::castRay(cameraPos, cameraDir, Constants::PlayerReach, world);
    if (!hitOpt) {
        return;
    }

    const auto&[blockPos, face] = *hitOpt;

    glm::ivec3 placePos = blockPos;
    switch (face) {
        case XP: placePos.x += 1; break;
        case XM: placePos.x -= 1; break;
        case YP: placePos.y += 1; break;
        case YM: placePos.y -= 1; break;
        case ZP: placePos.z += 1; break;
        case ZM: placePos.z -= 1; break;
    }

    if (!world.hasBlockAt(placePos)) {
        world.setBlockAt(placePos, blockType);
    }
}