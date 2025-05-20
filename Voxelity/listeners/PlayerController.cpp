//
// Created by adrian on 18/05/25.
//

#include "PlayerController.h"

#include <iostream>

#include "../voxelWorld/WorldChunkData.h"
#include "rendering/camera/CameraView.h"
#include "voxelWorld/utils/Raycaster.h"

PlayerController::PlayerController(CameraView& camera, WorldChunkData& world) : m_camera(camera), m_world(world) {}

void PlayerController::onEvent(Event& e) {
    if (e.type == EventType::MouseButtonPressed) {
        if (e.mouseButtonEvent.button == GLFW_MOUSE_BUTTON_LEFT)
            tryBreakBlock(m_camera.getPosition(), m_camera.getFront(), m_world);
        else if (e.mouseButtonEvent.button == GLFW_MOUSE_BUTTON_RIGHT) {
            tryPlaceBlock(m_camera.getPosition(), m_camera.getFront(), m_world, BlockType::Stone);
        }
    }
}

void PlayerController::tryBreakBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, WorldChunkData& world) {
    auto hitOpt = Raycaster::castRay(cameraPos, cameraDir, Constants::PlayerReach, world);
    if (!hitOpt) return;

    const auto&[blockPos, face] = *hitOpt;

    world.removeBlock(blockPos);
    std::cout << "Break" << std::endl;
}

void PlayerController::tryPlaceBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, WorldChunkData& world, const BlockType blockType) {
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
        std::cout << "Place" << std::endl;
    }
}