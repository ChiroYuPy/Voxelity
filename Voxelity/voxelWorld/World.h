//
// Created by adrian on 14/05/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <unordered_map>

#include "Voxel.h"
#include "../rendering/shader/Shader.h"
#include "textures/Texture.h"
#include "voxelWorld/generators/IWorldGenerator.h"

#include "voxelWorld/Chunk.h"

class Texture;
class VoxelFace;

class World {
public:
    explicit World();

    void generate(int cx, int cy, int cz);

    void generateChunk(Chunk *chunk) const;

    void generateFromPosition(glm::ivec3 position);

    void render(const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & lightDirection, const glm::vec3 & lightColor, const glm::vec3 & ambientColor) const;

    void prepareShader(const glm::mat4& view,
                              const glm::mat4& projection,
                              const glm::vec3& lightDir,
                              const glm::vec3& lightCol,
                              const glm::vec3& ambientCol) const;

    void prepareTextures() const;

    void update() const;

    Chunk* getChunkAt(int cx, int cy, int cz) const;

    const Voxel* getVoxelAt(int vx, int vy, int vz) const;

private:
    std::unique_ptr<IWorldGenerator> generator;
    std::unordered_map<unsigned long, std::unique_ptr<Chunk>> chunks;

    std::unique_ptr<Texture> textureAtlas;
    std::unique_ptr<Shader> chunkShader;

    static unsigned long chunkKey(int cx, int cy, int cz);

    glm::ivec3 keyToPos(unsigned long key);
};

#endif //WORLD_H
