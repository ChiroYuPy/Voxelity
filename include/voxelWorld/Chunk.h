//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <GLT.h>

#include <array>
#include <memory>

#include "Voxel.h"

class Mesh;
constexpr int CHUNK_SIZE = 16;

enum class Direction {
    POS_X, NEG_X, POS_Y, NEG_Y, POS_Z, NEG_Z
};

class Chunk {
public:
    Chunk(int cx, int cy, int cz);

    Chunk* getNeighbor(Direction direction);

    void setDirty(bool dirty_);

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] Voxel get(int x, int y, int z) const;
    void set(int x, int y, int z, Voxel voxel);

    void set(int x, int y, int z, BlockType blockType);

    [[nodiscard]] glm::ivec3 getPosition() const;

    [[nodiscard]] std::shared_ptr<Mesh> getMesh() const;

    void updateMesh();

private:
    void buildMesh() const;

    bool dirty;

    glm::ivec3 position;
    std::array<Voxel, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels;
    std::shared_ptr<Mesh> mesh;

    static int index(int x, int y, int z);
};

#endif //CHUNK_H
