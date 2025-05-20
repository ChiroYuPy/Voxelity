//
// Created by adrian on 16/05/25.
//

#include "WorldChunkData.h"

#include "chunk/Chunk.h"
#include "core/Application.h"
#include "core/Constants.h"

namespace {
    int floorDiv(const int a, const int b) {
        return (a >= 0) ? (a / b) : ((a - b + 1) / b);
    }

    constexpr int OFFSET = 1 << 20;

    unsigned long chunkKey(const int cx, const int cy, const int cz) {
        const unsigned long ux = static_cast<unsigned long>(cx + OFFSET) & 0x1FFFFF;
        const unsigned long uy = static_cast<unsigned long>(cy + OFFSET) & 0x1FFFFF;
        const unsigned long uz = static_cast<unsigned long>(cz + OFFSET) & 0x1FFFFF;

        return (ux << 42) | (uy << 21) | uz;
    }

    struct ChunkVoxelPos {
        glm::ivec3 chunkPos;
        glm::ivec3 localPos;
    };

    ChunkVoxelPos decomposeWorldPos(const glm::ivec3& worldPos) {
        constexpr int cs = Constants::ChunkSize;

        const int cx = floorDiv(worldPos.x, cs);
        const int cy = floorDiv(worldPos.y, cs);
        const int cz = floorDiv(worldPos.z, cs);

        const int lx = ((worldPos.x % cs) + cs) % cs;
        const int ly = ((worldPos.y % cs) + cs) % cs;
        const int lz = ((worldPos.z % cs) + cs) % cs;

        return { {cx, cy, cz}, {lx, ly, lz} };
    }
}

Chunk* WorldChunkData::getChunkAt(const glm::ivec3& pos) {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    const auto it = chunks.find(key);
    return (it != chunks.end()) ? it->second.get() : nullptr;
}

bool WorldChunkData::hasChunkAt(const glm::ivec3& pos) const {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    return chunks.contains(key);
}

bool WorldChunkData::hasBlockAt(const glm::ivec3& pos) const {
    const Voxel* voxel = getVoxelAt(pos);
    return voxel && voxel->getType() != BlockType::Air;
}

BlockType WorldChunkData::getBlockAt(const glm::ivec3& pos) const {
    const Voxel* voxel = getVoxelAt(pos);
    return voxel ? voxel->getType() : BlockType::Air;
}

void WorldChunkData::setBlockAt(const glm::ivec3& pos, const BlockType type) {
    const auto [chunkPos, localPos] = decomposeWorldPos(pos);
    const auto it = chunks.find(chunkKey(chunkPos.x, chunkPos.y, chunkPos.z));
    if (it == chunks.end()) return;

    const Voxel voxel(type);
    it->second->setState(ChunkState::MeshDirty);
    it->second->getData().set(localPos.x, localPos.y, localPos.z, voxel);
}

void WorldChunkData::removeBlock(const glm::ivec3 pos) {
    setBlockAt(pos, BlockType::Air);
}

std::vector<glm::ivec3> WorldChunkData::getAllChunkPositions() const {
    std::vector<glm::ivec3> result;
    for (const auto &chunk: chunks | std::views::values) {
        result.push_back(chunk->getPosition());
    }
    return result;
}

void WorldChunkData::addChunk(std::unique_ptr<Chunk> chunk) {
    const glm::ivec3 pos = chunk->getPosition();
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    chunks[key] = std::move(chunk);
}

void WorldChunkData::removeChunk(const glm::ivec3& pos) {
    const auto key = chunkKey(pos.x, pos.y, pos.z);
    chunks.erase(key);
}

const Voxel* WorldChunkData::getVoxelAt(const glm::ivec3& worldPos) const {
    const auto [chunkPos, localPos] = decomposeWorldPos(worldPos);
    const auto it = chunks.find(chunkKey(chunkPos.x, chunkPos.y, chunkPos.z));
    if (it == chunks.end()) return nullptr;

    return &it->second->getData().get(localPos.x, localPos.y, localPos.z);
}