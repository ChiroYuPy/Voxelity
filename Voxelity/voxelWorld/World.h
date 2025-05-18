//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "chunk/ChunkLoader.h"
#include "chunk/ChunkManager.h"
#include "chunk/ChunkRenderer.h"
#include "meshers/IChunkMesher.h"
#include "generators/IWorldGenerator.h"
#include "threads/generation/ChunkGenerationThread.h"

class VoxelFace;

class  World {
public:
    explicit World(std::unique_ptr<IChunkMesher> mesher, std::unique_ptr<IWorldGenerator> generator);

    void updateFromPlayerPosition(const glm::ivec3& playerWorldPos);

    void render(const glm::vec3& cameraPosition, const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor);

    void updateMeshes();

    void update();

private:
    ChunkManager chunkManager;
    ChunkRenderer chunkRenderer;
    ChunkLoader chunkLoader;
    std::unique_ptr<IChunkMesher> mesher;

    ChunkGenerationThread generationThread;
};

#endif //WORLD_H
