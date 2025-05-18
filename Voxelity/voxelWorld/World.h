//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "ChunkRequestManager.h"
#include "ChunkRenderer.h"
#include "meshers/IChunkMeshBuilder.h"
#include "generators/IWorldGenerator.h"
#include "threads/generation/ChunkGenerationThread.h"

class VoxelFace;

class  World {
public:
    explicit World(std::unique_ptr<IChunkMeshBuilder> mesher, std::unique_ptr<IWorldGenerator> generator);

    ~World();

    void updateFromPlayerPosition(const glm::ivec3& playerWorldPos) const;

    void render(const glm::vec3& cameraPosition, const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor) const;

    void updateMeshes() const;

    void update() const;

private:
    std::unique_ptr<IChunkMeshBuilder> meshBuilder;

    std::unique_ptr<WorldChunkData> chunkData;
    std::unique_ptr<ChunkRenderer> chunkRenderer;
    std::unique_ptr<ChunkRequestManager> chunkLoader;

    std::unique_ptr<ChunkGenerationThread> generationThread;
    //TODO meshingThread
};

#endif //WORLD_H
