//
// Created by adrian on 17/05/25.
//

#include "ChunkGenerationRequestManager.h"

#include <ranges>
#include <iostream>
#include <vector>

#include "voxelWorld/generators/IWorldGenerator.h"
#include "voxelWorld/WorldChunkData.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "voxelWorld/chunk/Chunk.h"
#include "threads/generation/ChunkGenerationThread.h"

ChunkGenerationRequestManager::ChunkGenerationRequestManager(std::unique_ptr<IChunkGenerator> generator) {
    generationThread = std::make_unique<ChunkGenerationThread>(std::move(generator));

    generationThread->start();
}

ChunkGenerationRequestManager::~ChunkGenerationRequestManager() {
    generationThread->stop();
}

void ChunkGenerationRequestManager::updateChunksAround(const glm::ivec3& playerChunkPos, WorldChunkData& chunkManager) {
    if (lastChunkPosition && *lastChunkPosition == playerChunkPos) return;
    lastChunkPosition = playerChunkPos;

    // load missing chunks in the area
    for (int x = playerChunkPos.x - Constants::RenderDistance; x <= playerChunkPos.x + Constants::RenderDistance; ++x) {
        for (int y = 0; y < Constants::WorldChunkHeight; ++y) {
            for (int z = playerChunkPos.z - Constants::RenderDistance; z <= playerChunkPos.z + Constants::RenderDistance; ++z) {
                glm::ivec3 pos{x, y, z};
                if (!chunkManager.hasChunkAt(pos) && isWithinRenderDistance(playerChunkPos, pos)) {
                    generateChunkAt(pos, chunkManager);
                }
            }
        }
    }

    // supress too far chunks from the area
    std::vector<glm::ivec3> toRemove;
    for (const auto& chunkPtr : chunkManager.chunks | std::views::values) {
        const glm::ivec3& pos = chunkPtr->getPosition();

        if (!isWithinRenderDistance(playerChunkPos, pos)) {
            toRemove.push_back(pos);
        }
    }

    for (const auto& pos : toRemove) {
        chunkManager.removeChunk(pos);
    }

    std::cout << "Chunks loaded: " << chunkManager.chunks.size()
              << " | Memory usage: " << static_cast<float>(chunkManager.chunks.size()) * 0.015625f << " MB\n";
}

void ChunkGenerationRequestManager::processReadyChunks(WorldChunkData& worldChunkData) const {
    glm::ivec3 pos;
    ChunkData data;
    while (generationThread->pollReadyElements(pos, data)) {
        if (!worldChunkData.hasChunkAt(pos)) continue;

        applyChunkData(pos, data, worldChunkData);
        updateNeighbors(pos, worldChunkData);
    }
}

void ChunkGenerationRequestManager::applyChunkData(const glm::ivec3& pos, const ChunkData& data, WorldChunkData& worldChunkData) {
    const auto chunk = worldChunkData.getChunkAt(pos);
    chunk->setData(data);
    chunk->setState(ChunkState::Generated);
}

void ChunkGenerationRequestManager::updateNeighbors(const glm::ivec3& pos, WorldChunkData& worldChunkData) {
    Chunk* chunk = worldChunkData.getChunkAt(pos);
    for (const auto& dir : DIRECTIONS) {
        const glm::ivec3 neighborPos = pos + getNormal(dir);
        Chunk* neighbor = worldChunkData.getChunkAt(neighborPos);
        if (neighbor) {
            chunk->setNeighbor(dir, neighbor);
            neighbor->setNeighbor(getOpposite(dir), chunk);
            neighbor->setState(ChunkState::MeshDirty);
        }
    }
}

void ChunkGenerationRequestManager::generateChunkAt(const glm::ivec3& pos, WorldChunkData& manager) const {
    auto chunk = std::make_unique<Chunk>(pos);
    chunk->setState(ChunkState::QueuedForGeneration);
    manager.addChunk(std::move(chunk));
    generationThread->enqueueElement(pos);

}

bool ChunkGenerationRequestManager::isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos) {
    const int dx = pos.x - center.x;
    const int dz = pos.z - center.z;
    return dx * dx + dz * dz <= Constants::RenderDistance * Constants::RenderDistance;
}