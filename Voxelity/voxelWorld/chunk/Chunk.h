//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <atomic>
#include <GLT.h>

#include "VoxelStorage.h"
#include "math/BlockFace.h"
#include "ChunkMesh.h"

class IChunkMeshBuilder;

enum class ChunkState {
    UnGenerated,
    QueuedForGeneration,
    Generating,
    MeshDirty,
    Meshing,
    ReadyToRender
};

inline const char* toString(const ChunkState state) {
    switch (state) {
        case ChunkState::UnGenerated:    return "UnGenerated";
        case ChunkState::Generating:     return "Generating";
        case ChunkState::MeshDirty:      return "MeshDirty";
        case ChunkState::Meshing:        return "Meshing";
        case ChunkState::ReadyToRender:  return "ReadyToRender";
        default:                         return "Unknown";
    }
}

class Chunk {
    glm::ivec3 position;
    VoxelStorage data;
    ChunkMesh mesh;
    mutable std::mutex dataMutex;
    mutable std::mutex meshMutex;
    std::atomic<ChunkState> state;

    bool empty;

    std::array<Chunk*, 6> neighbors = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

    void updateEmptyFlag();

public:
    explicit Chunk(glm::ivec3 position);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] glm::ivec3 getWorldPosition() const;

    [[nodiscard]] VoxelStorage& getData();

    [[nodiscard]] const VoxelStorage &getData() const;

    ChunkMesh &getMesh();

    [[nodiscard]] const ChunkMesh &getMesh() const;

    void setData(const VoxelStorage& newData);

    void setMeshData(const ChunkMeshData& newMeshData);

    [[nodiscard]] Chunk* getNeighbor(BlockFace direction) const;
    void setNeighbor(BlockFace direction, Chunk* neighbor);

    [[nodiscard]] std::array<Chunk*, 6> getNeighbors() const;

    void setState(ChunkState newState);

    [[nodiscard]] ChunkState getState() const;

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool isEmpty() const;

    void render() const;
};

#endif //CHUNK_H
