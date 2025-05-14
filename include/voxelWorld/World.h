//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "rendering/Shader.h"
#include "voxelWorld/generators/IWorldGenerator.h"

class VoxelFace;

class World {
public:
    explicit World();

    void generate(int cx, int cy, int cz);

    void generateFromPosition(glm::ivec3 position);

    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDirection) const;

    void update() const;

private:
    std::unique_ptr<IWorldGenerator> generator;
    std::unordered_map<unsigned long, std::unique_ptr<Chunk>> chunks;

    std::unique_ptr<Shader> chunkShader;

    static unsigned long chunkKey(int cx, int cy, int cz);
    static bool isFaceVisible(int x, int y, int z, const Chunk& chunk, int face);
};

#endif //WORLD_H
