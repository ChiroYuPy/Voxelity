//
// Created by adrian on 16/05/25.
//

#include "ChunkManager.h"

#include "Chunk.h"
#include "core/Constants.h"

static int floorDiv(const int a, const int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

Chunk* ChunkManager::getChunkAt(const glm::ivec3& pos) {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    const auto it = chunks.find(key);
    return it != chunks.end() ? it->second.get() : nullptr;
}

bool ChunkManager::hasChunkAt(const glm::ivec3& pos) const {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    return chunks.contains(key);
}

void ChunkManager::addChunk(std::unique_ptr<Chunk> chunk) {
    const glm::ivec3 pos = chunk->getPosition();
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    chunks[key] = std::move(chunk);
}

void ChunkManager::removeChunk(const glm::ivec3& pos) {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    chunks.erase(key);
}

const Voxel* ChunkManager::getVoxelAt(const glm::ivec3& worldPos) const {
    const int cx = floorDiv(worldPos.x, Constants::ChunkSize);
    const int cy = floorDiv(worldPos.y, Constants::ChunkSize);
    const int cz = floorDiv(worldPos.z, Constants::ChunkSize);

    const int xInChunk = ((worldPos.x % Constants::ChunkSize) + Constants::ChunkSize) % Constants::ChunkSize;
    const int yInChunk = ((worldPos.y % Constants::ChunkSize) + Constants::ChunkSize) % Constants::ChunkSize;
    const int zInChunk = ((worldPos.z % Constants::ChunkSize) + Constants::ChunkSize) % Constants::ChunkSize;

    const auto key = chunkKey(cx, cy, cz);
    const auto it = chunks.find(key);
    if (it == chunks.end()) return nullptr;

    return &it->second->getData().get(xInChunk, yInChunk, zInChunk);
}

unsigned long ChunkManager::chunkKey(const int cx, const int cy, const int cz) {
    constexpr int OFFSET = 1 << 20;

    const unsigned long ux = static_cast<unsigned long>(cx + OFFSET) & 0x1FFFFF;
    const unsigned long uy = static_cast<unsigned long>(cy + OFFSET) & 0x1FFFFF;
    const unsigned long uz = static_cast<unsigned long>(cz + OFFSET) & 0x1FFFFF;

    return (ux << 42) | (uy << 21) | uz;
}