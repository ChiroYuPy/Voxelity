//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <atomic>
#include <GLT.h>

#include "components/ChunkData.h"
#include "math/BlockFace.h"
#include "components/ChunkMesh.h"

class IChunkMesher;

enum class ChunkState {
    UnGenerated,
    Generating,
    Generated,
    MeshDirty,
    Meshing,
    ReadyToRender
};

inline const char* toString(const ChunkState state) {
    switch (state) {
        case ChunkState::UnGenerated:    return "UnGenerated";
        case ChunkState::Generating:     return "Generating";
        case ChunkState::Generated:      return "Generated";
        case ChunkState::MeshDirty:      return "MeshDirty";
        case ChunkState::Meshing:        return "Meshing";
        case ChunkState::ReadyToRender:  return "ReadyToRender";
        default:                         return "Unknown";
    }
}

class Chunk {
    glm::ivec3 position;
    ChunkData data;
    ChunkMesh mesh;

    bool empty;

    std::atomic<ChunkState> state;

    Chunk* neighbors[6] = {nullptr};

    void updateEmptyFlag();

public:
    explicit Chunk(glm::ivec3 position);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] glm::ivec3 getWorldPosition() const;

    [[nodiscard]] const ChunkMesh &getMesh() const;

    [[nodiscard]] const ChunkData &getData() const;

    [[nodiscard]] ChunkMesh& getMesh();

    [[nodiscard]] ChunkData& getData();

    void setData(const ChunkData& newData);

    void setMesh(const ChunkMesh& newMesh);

    [[nodiscard]] Chunk* getNeighbor(BlockFace direction) const;
    void setNeighbor(BlockFace direction, Chunk* neighbor);

    void setState(ChunkState newState);

    [[nodiscard]] ChunkState getState() const;

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool isEmpty() const;
};

#endif //CHUNK_H
