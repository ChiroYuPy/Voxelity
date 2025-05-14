//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "Chunk.h"

#include "voxelWorld/generators/IWorldGenerator.h"

class VoxelFace;

class World {
public:
    World();

    void generate(int cx, int cy, int cz);

    std::vector<VoxelFace> generateFaceInstances() const;

private:
    std::unique_ptr<IWorldGenerator> generator;
    std::unordered_map<unsigned long, std::unique_ptr<Chunk>> chunks;

    static unsigned long chunkKey(int cx, int cy, int cz);
    static bool isFaceVisible(int x, int y, int z, const Chunk& chunk, int face);
};

#endif //WORLD_H
