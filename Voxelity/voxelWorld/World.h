//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>

#include "ChunkManager.h"
#include "ChunkRenderer.h"
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
    ChunkRenderer chunkRenderer;

    Frustum frustum;

    std::unique_ptr<IWorldGenerator> generator;

    std::unique_ptr<Texture> textureAtlas;
    std::unique_ptr<Shader> chunkShader;

    void generateChunk(Chunk *chunk);
};

#endif //WORLD_H
