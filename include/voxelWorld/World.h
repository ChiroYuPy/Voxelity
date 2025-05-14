//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>

#include "Voxel.h"
#include "rendering/Shader.h"
#include "textures/Texture.h"
#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

class Texture;
class VoxelFace;

class World {
public:
    explicit World();

    void generate(int cx, int cy, int cz);

    void generateFromPosition(glm::ivec3 position);

    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDirection) const;

    void update() const;

    Chunk* getChunkAt(int cx, int cy, int cz) const;

    const Voxel* getVoxelAt(int vx, int vy, int vz) const;

private:
    std::unique_ptr<IWorldGenerator> generator;
    std::unordered_map<unsigned long, std::unique_ptr<Chunk>> chunks;

    std::unique_ptr<Texture> textureAtlas;
    std::unique_ptr<Shader> chunkShader;

    static unsigned long chunkKey(int cx, int cy, int cz);
};

#endif //WORLD_H
