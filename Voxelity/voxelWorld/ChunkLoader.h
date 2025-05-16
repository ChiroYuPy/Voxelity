//
// Created by adrian on 17/05/25.
//

#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <GLT.h>

#include <optional>

class ChunkManager;
class IWorldGenerator;

class ChunkLoader {
public:
    void updateChunksAround(const glm::ivec3& playerChunkPos,
                            ChunkManager& manager,
                            IWorldGenerator& generator);

private:
    std::optional<glm::ivec3> lastChunkPosition;

    void generateChunkAt(const glm::ivec3& pos,
                         ChunkManager& manager,
                         IWorldGenerator& generator);

    bool isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos);
};

#endif //CHUNKLOADER_H
