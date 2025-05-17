//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <GLT.h>
#include <mutex>

#include "ChunkData.h"
#include "math/Direction.h"
#include "rendering/ChunkMesh.h"

enum class ChunkState {
    Unloaded,   // empty chunk, chunkData fill of air, chunkMesh without any vertices
    Generating, // chunkData generation
    Generated,
    Meshing,    // chunkMesh calculation
    Meshed,
    Loaded      // Ready to render
};

class Chunk {
    glm::ivec3 position;
    ChunkData data;
    ChunkMesh mesh;

    bool dirty;
    bool empty;

    ChunkState state;
    std::mutex mutex;

    Chunk* neighbors[6] = {nullptr};

    void buildMesh();

    void updateEmptyFlag();

    void unmarkDirty();

public:
    explicit Chunk(glm::ivec3 position);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] glm::ivec3 getWorldPosition() const;

    [[nodiscard]] const ChunkMesh& getMesh() const;

    [[nodiscard]] const ChunkData& getData() const;

    void setData(const ChunkData& newData);

    void updateMesh();

    [[nodiscard]] Chunk* getNeighbor(Direction direction) const;
    void setNeighbor(Direction direction, Chunk* neighbor);

    void setState(ChunkState s);

    [[nodiscard]] ChunkState getState() const;

    void markDirty();

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool isEmpty() const;

};

#endif //CHUNK_H
