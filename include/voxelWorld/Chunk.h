//
// Created by adrian on 14/05/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <GLT.h>

#include <array>
#include <memory>

#include "Voxel.h"

class World;
class Mesh;
constexpr int CHUNK_SIZE = 16;

class Chunk {
public:
    Chunk(int cx, int cy, int cz, const World* world);

    void setDirty(bool dirty_);
    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] const Voxel* getVoxelAt(int x, int y, int z) const;
    void set(int x, int y, int z, Voxel voxel);
    void set(int x, int y, int z, BlockType blockType);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] std::shared_ptr<Mesh> getMesh() const;
    void updateMesh();

    [[nodiscard]] Chunk* getNeighbor(uint8_t direction) const;
    void setNeighbor(uint8_t direction, Chunk* neighbor);

private:
    void buildMesh() const;

    const World *world;

    bool dirty;
    glm::ivec3 position;
    std::array<Voxel, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels;
    std::shared_ptr<Mesh> mesh;
    Chunk* neighbors[6] = {nullptr};

    static int index(int x, int y, int z);
};

#endif //CHUNK_H
