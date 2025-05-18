//
// Created by adrian on 17/05/25.
//

#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <GLT.h>
#include <memory>

#include <optional>

struct ChunkData;
class ChunkGenerationThread;
class WorldChunkData;
class IWorldGenerator;

class ChunkGenerationRequestManager {
public:
    explicit ChunkGenerationRequestManager(std::unique_ptr<IWorldGenerator> generator);
    ~ChunkGenerationRequestManager();

    void updateChunksAround(const glm::ivec3 & playerChunkPos, WorldChunkData &worldChunkData);

    void processReadyChunks(WorldChunkData& worldChunkData) const;

    static void applyChunkData(const glm::ivec3& pos, const ChunkData& data, WorldChunkData& worldChunkData);

    static void updateNeighbors(const glm::ivec3& pos, WorldChunkData& worldChunkData);

private:
    std::unique_ptr<ChunkGenerationThread> generationThread;

    std::optional<glm::ivec3> lastChunkPosition;

    void generateChunkAt(const glm::ivec3 & pos, WorldChunkData &worldChunkData) const;

    static bool isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos);
};

#endif //CHUNKLOADER_H
