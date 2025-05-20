//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "ChunkManager.h"
#include "managers/ChunkGenerationRequestManager.h"
#include "WorldChunkRenderer.h"
#include "meshBuilders/IChunkMeshBuilder.h"
#include "generators/IWorldGenerator.h"
#include "threads/generation/ChunkGenerationThread.h"

class ChunkMeshingRequestManager;
class ChunkMeshingThread;
class VoxelFace;

class  World {
public:
    explicit World(std::unique_ptr<IChunkMeshBuilder> meshBuilder_, std::unique_ptr<IChunkGenerator> generator);

    ~World();

    void updateFromPlayerPosition(const glm::ivec3& playerWorldPos) const;

    void render(const glm::vec3& cameraPosition, const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor) const;

    void update() const;

    WorldChunkData& getWorldChunkData() const;

private:

    std::unique_ptr<IChunkMeshBuilder> meshBuilder;

    std::unique_ptr<WorldChunkData> worldChunkData;
    std::unique_ptr<WorldChunkRenderer> chunkRenderer;

    std::unique_ptr<ChunkGenerationRequestManager> chunkGenerationRequestManager;
    std::unique_ptr<ChunkMeshingRequestManager> chunkMeshingRequestManager;

    std::unique_ptr<ChunkMeshingThread> chunkMeshingThread;
};

#endif //WORLD_H
