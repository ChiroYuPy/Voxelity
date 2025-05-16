//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <GLT.h>

#include <array>
#include <memory>

#include "Voxel.h"
#include "math/Direction.h"

class ChunkMesh;

class Chunk {
public:
    static constexpr int SIZE = 16;

private:
    std::array<Voxel, SIZE * SIZE * SIZE> voxels;

    glm::ivec3 position;
    bool dirty;
    bool empty;

    Chunk* neighbors[6] = {nullptr};

    void buildMesh() const;

    std::shared_ptr<ChunkMesh> mesh;

    static unsigned int index(unsigned int x, unsigned int y, unsigned int z);
    static unsigned int index(glm::uvec3 position);

    void updateEmptyFlag();

public:
    explicit Chunk(glm::ivec3 position);

    [[nodiscard]] const Voxel* at(unsigned int x, unsigned int y, unsigned int z) const;
    void set(unsigned int x, unsigned int y, unsigned int z, BlockType blockType);

    [[nodiscard]] const Voxel* at(glm::uvec3 position) const;
    void set(glm::uvec3 position, BlockType blockType);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] glm::ivec3 getWorldPosition() const;

    [[nodiscard]] std::shared_ptr<ChunkMesh> getMesh() const;
    void updateMesh();

    [[nodiscard]] Chunk* getNeighbor(Direction direction) const;
    void setNeighbor(Direction direction, Chunk* neighbor);

    void markDirty();
    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool isEmpty() const;

    void fill(BlockType blockType);
};

#endif //CHUNK_H
