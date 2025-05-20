//
// Created by adrian on 16/05/25.
//

#ifndef WORLDCHUNKDATA_H
#define WORLDCHUNKDATA_H

#include <GLT.h>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "chunk/Chunk.h"

using ChunkKey = unsigned long;

class WorldChunkData {
public:
    std::unordered_map<ChunkKey, std::unique_ptr<Chunk>> chunks;

    Chunk* getChunkAt(const glm::ivec3& pos);
    const Voxel* getVoxelAt(const glm::ivec3& worldPos) const;

    void addChunk(std::unique_ptr<Chunk> chunk);
    void removeChunk(const glm::ivec3& pos);
    bool hasChunkAt(const glm::ivec3& pos) const;

    bool hasBlockAt(const glm::ivec3& pos) const;
    BlockType getBlockAt(const glm::ivec3 & pos) const;
    void setBlockAt(const glm::ivec3& pos, BlockType type);

    void removeBlock(glm::ivec3 pos);

    std::vector<glm::ivec3> getAllChunkPositions() const;
};

#endif //WORLDCHUNKDATA_H
