//
// Created by adrian on 17/05/25.
//

#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <GLT.h>

#include <optional>

class ChunkGenerationThread;
class WorldChunkData;
class IWorldGenerator;

class ChunkRequestManager {
public:
    void updateChunksAround(const glm::ivec3& playerChunkPos, WorldChunkData& chunkManager, ChunkGenerationThread& generationThread);

private:
    std::optional<glm::ivec3> lastChunkPosition;

    static void generateChunkAt(const glm::ivec3 & pos, ChunkGenerationThread &generationThread, WorldChunkData &manager);

    static bool isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos);
};

#endif //CHUNKLOADER_H
