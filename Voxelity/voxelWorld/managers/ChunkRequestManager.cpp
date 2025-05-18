//
// Created by adrian on 17/05/25.
//

#include "ChunkRequestManager.h"

#include <ranges>
#include <iostream>
#include <vector>

#include "../WorldChunkData.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "../chunk/Chunk.h"
#include "threads/generation/ChunkGenerationThread.h"

class ChunkGenerationThread;

void ChunkRequestManager::updateChunksAround(const glm::ivec3& playerChunkPos, WorldChunkData& chunkManager, ChunkGenerationThread& generationThread) {
    PROFILE_FUNCTION();

    if (lastChunkPosition && *lastChunkPosition == playerChunkPos) return;
    lastChunkPosition = playerChunkPos;

    // Chargement des chunks dans la zone autour du joueur
    for (int x = playerChunkPos.x - Constants::RenderDistance; x <= playerChunkPos.x + Constants::RenderDistance; ++x) {
        for (int y = 0; y < Constants::RenderHeight; ++y) {
            for (int z = playerChunkPos.z - Constants::RenderDistance; z <= playerChunkPos.z + Constants::RenderDistance; ++z) {
                glm::ivec3 pos{x, y, z};
                if (!chunkManager.hasChunkAt(pos) && isWithinRenderDistance(playerChunkPos, pos)) {
                    generateChunkAt(pos, generationThread, chunkManager);
                }
            }
        }
    }

    // Suppression des chunks trop éloignés
    std::vector<glm::ivec3> toRemove;
    for (const auto& chunkPtr : chunkManager.chunks | std::views::values) {
        const glm::ivec3& pos = chunkPtr->getPosition();

        // Ici, y doit rester dans [0, RenderHeight) (pas négatif)
        if (!isWithinRenderDistance(playerChunkPos, pos) ||
            pos.y < 0 || pos.y >= Constants::RenderHeight) {
            toRemove.push_back(pos);
            }
    }

    for (const auto& pos : toRemove) {
        chunkManager.removeChunk(pos);
    }

    std::cout << "Chunks loaded: " << chunkManager.chunks.size()
              << " | Memory usage: " << static_cast<float>(chunkManager.chunks.size()) * 0.015625f << " MB\n";
}

void ChunkRequestManager::generateChunkAt(const glm::ivec3& pos, ChunkGenerationThread& generationThread, WorldChunkData& manager) {
    auto chunk = std::make_unique<Chunk>(pos);
    manager.addChunk(std::move(chunk));
    generationThread.enqueuePosition(pos);
}

bool ChunkRequestManager::isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos) {
    const int dx = pos.x - center.x;
    const int dz = pos.z - center.z;
    // Distance euclidienne au carré sur le plan XZ
    return dx * dx + dz * dz <= Constants::RenderDistance * Constants::RenderDistance;
}