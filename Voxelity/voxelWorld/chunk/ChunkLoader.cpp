//
// Created by adrian on 17/05/25.
//

#include "ChunkLoader.h"

#include <ranges>
#include <iostream>
#include <vector>

#include "ChunkManager.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "Chunk.h"
#include "threads/generation/ChunkGenerationThread.h"

class ChunkGenerationThread;

void ChunkLoader::updateChunksAround(const glm::ivec3& playerChunkPos, ChunkManager& manager, ChunkGenerationThread& generationThread) {
    PROFILE_FUNCTION();

    if (lastChunkPosition && *lastChunkPosition == playerChunkPos) return;
    lastChunkPosition = playerChunkPos;

    for (int x = playerChunkPos.x - Constants::RenderDistance; x <= playerChunkPos.x + Constants::RenderDistance; ++x) {
        for (int y = 0; y < Constants::RenderHeight; ++y) {
            for (int z = playerChunkPos.z - Constants::RenderDistance; z <= playerChunkPos.z + Constants::RenderDistance; ++z) {
                glm::ivec3 pos{x, y, z};
                if (!manager.hasChunkAt(pos) && isWithinRenderDistance(playerChunkPos, pos)) {
                    generateChunkAt(pos, generationThread);
                }
            }
        }
    }

    std::vector<glm::ivec3> toRemove;
    for (const auto& chunkPtr : manager.chunks | std::views::values) {
        const glm::ivec3& pos = chunkPtr->getPosition();
        if (!isWithinRenderDistance(playerChunkPos, pos) ||
            pos.y < -Constants::RenderHeight || pos.y >= Constants::RenderHeight) {
            toRemove.push_back(pos);
        }
    }

    for (const auto& pos : toRemove) {
        manager.removeChunk(pos);
    }

    std::cout << "Chunks loaded: " << manager.chunks.size()
              << " | Memory usage: " << static_cast<float>(manager.chunks.size()) * 0.015625f << " MB\n";
}

void ChunkLoader::generateChunkAt(const glm::ivec3& pos, ChunkGenerationThread& generationThread) {
    generationThread.enqueuePosition(pos);
}

bool ChunkLoader::isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos) {
    const int dx = pos.x - center.x;
    const int dz = pos.z - center.z;
    return dx * dx + dz * dz <= Constants::RenderDistance * Constants::RenderDistance;
}