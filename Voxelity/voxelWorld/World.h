//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "ChunkLoader.h"
#include "ChunkManager.h"
#include "ChunkRenderer.h"
#include "voxelWorld/generators/IWorldGenerator.h"

class VoxelFace;

class  World {
public:
    explicit World(std::unique_ptr<IWorldGenerator> generator);

    void updateFromPlayerPosition(const glm::ivec3& playerWorldPos);

    void render(const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor);

    void update() const;

private:
    ChunkManager chunkManager;
    ChunkRenderer chunkRenderer;
    ChunkLoader chunkLoader;

    std::unique_ptr<IWorldGenerator> generator;
};

#endif //WORLD_H
