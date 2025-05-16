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
#include "generators/IWorldGenerator.h"
#include "voxelWorld/Chunk.h"

void ChunkLoader::updateChunksAround(const glm::ivec3& playerChunkPos,
                                     ChunkManager& manager,
                                     IWorldGenerator& generator) {
    PROFILE_FUNCTION();

    if (lastChunkPosition && *lastChunkPosition == playerChunkPos) return;
    lastChunkPosition = playerChunkPos;

    // Génération des nouveaux chunks autour du joueur
    for (int x = playerChunkPos.x - Constants::RenderDistance; x <= playerChunkPos.x + Constants::RenderDistance; ++x) {
        for (int y = -Constants::RenderHeight; y < Constants::RenderHeight; ++y) {
            for (int z = playerChunkPos.z - Constants::RenderDistance; z <= playerChunkPos.z + Constants::RenderDistance; ++z) {
                glm::ivec3 pos{x, y, z};
                if (!manager.hasChunkAt(pos) && isWithinRenderDistance(playerChunkPos, pos)) {
                    generateChunkAt(pos, manager, generator);
                }
            }
        }
    }

    // Suppression des chunks trop éloignés
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
              << " | Memory usage: " << manager.chunks.size() * 0.015625f << " MB\n";
}

void ChunkLoader::generateChunkAt(const glm::ivec3& pos,
                                  ChunkManager& manager,
                                  IWorldGenerator& generator) {
    auto chunk = std::make_unique<Chunk>(pos);
    const ChunkData data = generator.generate(pos * Constants::ChunkSize);
    chunk->setData(data);

    // Connexion des voisins
    for (const auto& dir : DIRECTIONS) {
        const glm::ivec3 neighborPos = pos + getNormal(dir);
        Chunk* neighbor = manager.getChunkAt(neighborPos);
        if (neighbor) {
            chunk->setNeighbor(dir, neighbor);
            neighbor->setNeighbor(getOpposite(dir), chunk.get());
            neighbor->markDirty(); // Rebuild pour mettre à jour les faces visibles
        }
    }

    manager.addChunk(std::move(chunk));
}

bool ChunkLoader::isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos) {
    const int dx = pos.x - center.x;
    const int dz = pos.z - center.z;
    return dx * dx + dz * dz <= Constants::RenderDistance * Constants::RenderDistance;
}