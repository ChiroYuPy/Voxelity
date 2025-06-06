//
// Created by adrian on 17/05/25.
//

#include "ChunkGenerationRequestManager.h"

#include <ranges>
#include <iostream>
#include <vector>

#include "voxelWorld/generators/IWorldGenerator.h"
#include "voxelWorld/ChunkStorage.h"
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

void ChunkGenerationRequestManager::updateChunksAround(const glm::ivec3& playerChunkPos, ChunkStorage& worldChunkData) {
    if (lastChunkPosition && *lastChunkPosition == playerChunkPos) return;
    lastChunkPosition = playerChunkPos;

    // load missing chunks in the area
    for (int x = playerChunkPos.x - Constants::RENDER_DISTANCE; x <= playerChunkPos.x + Constants::RENDER_DISTANCE; ++x) {
        for (int y = 0; y < Constants::WORLD_HEIGHT; ++y) {
            for (int z = playerChunkPos.z - Constants::RENDER_DISTANCE; z <= playerChunkPos.z + Constants::RENDER_DISTANCE; ++z) {
                if (glm::ivec3 pos{x, y, z}; !worldChunkData.hasChunkAt(pos) && isWithinRenderDistance(playerChunkPos, pos)) {
                    generateChunkAt(pos, worldChunkData);
                }
            }
        }
    }

    // supress too far chunks from the area
    std::vector<glm::ivec3> toRemove;
    for (const auto& chunkPtr : worldChunkData.chunks | std::views::values) {
        const glm::ivec3& pos = chunkPtr->getPosition();

        if (!isWithinRenderDistance(playerChunkPos, pos)) {
            toRemove.push_back(pos);
        }
    }

    for (const auto& pos : toRemove) {
        worldChunkData.removeChunk(pos);
    }

    std::cout << "Chunks loaded: " << worldChunkData.chunks.size()
              << " | Memory usage: " << static_cast<float>(worldChunkData.chunks.size()) * 0.015625f << " MB\n";
}

void ChunkGenerationRequestManager::processReadyChunks(ChunkStorage& worldChunkData) const {
    glm::ivec3 pos;
    VoxelStorage data;
    while (generationThread->pollReadyElements(pos, data)) {
        if (!worldChunkData.hasChunkAt(pos)) continue;

        applyChunkData(pos, data, worldChunkData);
        updateNeighbors(pos, worldChunkData);
    }
}

void ChunkGenerationRequestManager::applyChunkData(const glm::ivec3& pos, const VoxelStorage& data, ChunkStorage& worldChunkData) {
    const auto chunk = worldChunkData.getChunkAt(pos);
    chunk->setData(data);
    chunk->setState(ChunkState::MeshDirty);
}

void ChunkGenerationRequestManager::updateNeighbors(const glm::ivec3& pos, ChunkStorage& worldChunkData) {
    Chunk* chunk = worldChunkData.getChunkAt(pos);
    for (const auto& dir : DIRECTIONS) {
        const glm::ivec3 neighborPos = pos + getNormal(dir);
        if (Chunk* neighbor = worldChunkData.getChunkAt(neighborPos)) {
            chunk->setNeighbor(dir, neighbor);
            neighbor->setNeighbor(getOpposite(dir), chunk);
            if (neighbor->getState() == ChunkState::ReadyToRender) neighbor->setState(ChunkState::MeshDirty);
        }
    }
}

void ChunkGenerationRequestManager::generateChunkAt(const glm::ivec3& pos, ChunkStorage& worldChunkData) const {
    auto chunk = std::make_unique<Chunk>(pos);
    chunk->setState(ChunkState::QueuedForGeneration);
    worldChunkData.addChunk(std::move(chunk));
    generationThread->enqueueElement(pos);
}

bool ChunkGenerationRequestManager::isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos) {
    const int dx = pos.x - center.x;
    const int dz = pos.z - center.z;
    return dx * dx + dz * dz <= Constants::RENDER_DISTANCE * Constants::RENDER_DISTANCE;
}