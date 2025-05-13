//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "Chunk.h"

class VoxelFace;

class World {
public:
    World();

    Chunk& getOrCreateChunk(int cx, int cy, int cz);
    std::vector<VoxelFace> generateFaceInstances() const;

private:
    std::unordered_map<int64_t, std::unique_ptr<Chunk>> chunks;

    static int64_t chunkKey(int cx, int cy, int cz);

    static bool isFaceVisible(int x, int y, int z, const Chunk& chunk, int face);
};

#endif //WORLD_H
