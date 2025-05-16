//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>

#include "Voxel.h"
#include "rendering/shader/Shader.h"
#include "math/Frustum.h"
#include "textures/Texture.h"
#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

class Texture;
class VoxelFace;

class  World {
public:
    explicit World(std::unique_ptr<IWorldGenerator> generator);

    void generateFromPlayerPosition(const glm::ivec3& position);

    void generateFromChunkPosition(glm::ivec3 playerPosition);

    void render(const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor);

    void update() const;

    Chunk* getChunkAt(int cx, int cy, int cz) const;

    const Voxel* getVoxelAt(int vx, int vy, int vz) const;

private:
    Frustum frustum;

    std::unique_ptr<IWorldGenerator> generator;
    std::unordered_map<unsigned long, std::unique_ptr<Chunk>> chunks;

    std::unique_ptr<Texture> textureAtlas;
    std::unique_ptr<Shader> chunkShader;

    void generateChunk(Chunk *chunk) const;

    void prepareShader(const glm::mat4& view,
                              const glm::mat4& projection,
                              const glm::vec3& lightDir,
                              const glm::vec3& lightCol,
                              const glm::vec3& ambientCol) const;

    void prepareTextures() const;

    static unsigned long chunkKey(int cx, int cy, int cz);
};

#endif //WORLD_H
