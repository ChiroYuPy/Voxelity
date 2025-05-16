//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>

#include "ChunkManager.h"
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

private:
    ChunkManager chunkManager;

    Frustum frustum;

    std::unique_ptr<IWorldGenerator> generator;

    std::unique_ptr<Texture> textureAtlas;
    std::unique_ptr<Shader> chunkShader;

    void generateChunk(Chunk *chunk);

    void prepareShader(const glm::mat4& view,
                              const glm::mat4& projection,
                              const glm::vec3& lightDir,
                              const glm::vec3& lightCol,
                              const glm::vec3& ambientCol) const;

    void prepareTextures() const;
};

#endif //WORLD_H
