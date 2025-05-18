//
// Created by adrian on 18/05/25.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <GLT.h>

#include "blocks/BlockType.h"
#include "events/EventListener.h"

class ChunkManager;

class PlayerController final : public EventListener {
public:
    void onEvent(Event& e) override;

    static void tryBreakBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkManager& world);

    static void tryPlaceBlock(const glm::vec3& cameraPos, const glm::vec3& cameraDir, ChunkManager& world, BlockType blockType0);
};



#endif //PLAYERCONTROLLER_H
