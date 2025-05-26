//
// Created by adrian on 18/05/25.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <GLT.h>

#include "blocks/BlockType.h"
#include "events/EventListener.h"

class CameraView;
class ChunkStorage;

class PlayerController final : public EventListener {
public:
    PlayerController(CameraView& camera, ChunkStorage& world);

    void onEvent(Event& e) override;

    static void tryBreakBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkStorage& world);

    static void tryPlaceBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkStorage& world, BlockType blockType0);

private:
    CameraView& m_camera;
    ChunkStorage& m_world;
};



#endif //PLAYERCONTROLLER_H
