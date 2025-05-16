//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "core/Application.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "math/Frustum.h"
#include "rendering/ChunkMesh.h"
#include "rendering/shader/Shader.h"

inline int floorDiv(const int a, const int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

// TODO: DONE   Lazy Update ( dirty flag )
// TODO: DONE   Distance Culling ( Render Distance )
// TODO: DONE   Frustum Culling ( viewProj & chunk AABB )
// TODO: DONE   Face Culling ( isVisible() test )
// TODO: DONE   Basic Occlusion Culling ( isEmpty() & hasVisibleFaces() tests )
// TODO         Complete Occlusion Culling
// TODO         Level Of Details
// TODO         Greedy Meshing
// TODO         Multi-Threading -> world generation, chunk meshing, rendering

// TODO: DONE   VoxelFace Data Compression

// TODO         Minimize Shaders Calculs
// TODO         Optimize Shaders Uniforms

World::World(std::unique_ptr<IWorldGenerator> generator) : generator(std::move(generator)) {

    chunkShader = std::make_unique<Shader>(
        "../resources/shaders/vertex_shader.glsl",
        "../resources/shaders/fragment_shader.glsl");

    textureAtlas = std::make_unique<Texture>(
        "../resources/textures/atlas.png");

    textureAtlas->bind(0);
}

void World::render(const glm::mat4& view,
                   const glm::mat4& projection,
                   const glm::vec3& lightDirection,
                   const glm::vec3& lightColor,
                   const glm::vec3& ambientColor) {
    PROFILE_FUNCTION();
    prepareShader(view, projection, lightDirection, lightColor, ambientColor);
    prepareTextures();

    // update frustum
    const glm::mat4 viewProj = projection * view;
    frustum.update(viewProj);

    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) {
        const auto& chunk = *chunkPtr;
        // AABB calcul
        const glm::vec3 chunkWorldMin = chunk.getPosition() * Constants::ChunkSize; // GOULET ?
        constexpr glm::vec3 halfSize = glm::vec3(Constants::ChunkSize) * 0.5f;
        const glm::vec3 center = chunkWorldMin + halfSize;

        // frustum culling
        if (frustum.intersectsAABB(center, halfSize) && chunk.getMesh().hasVisibleFaces()) {
            chunkShader->setUniform("uChunkOffset", chunk.getWorldPosition());
            chunk.getMesh().render();
        }
    }
}

void World::prepareShader(const glm::mat4& view,
                          const glm::mat4& projection,
                          const glm::vec3& lightDir,
                          const glm::vec3& lightCol,
                          const glm::vec3& ambientCol) const {
    chunkShader->use();

    // vertex shader
    chunkShader->setUniform("uView", view);
    chunkShader->setUniform("uProjection", projection);

    // fragment shader
    chunkShader->setUniform("uLightDirection", glm::normalize(lightDir));
    chunkShader->setUniform("uLightColor", lightCol);
    chunkShader->setUniform("uAmbientColor", ambientCol);
}

void World::prepareTextures() const {
    glActiveTexture(GL_TEXTURE0);
    textureAtlas->bind(0);
    chunkShader->setUniform("uAtlas", 0);
}

void World::update() const {
    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) chunkPtr->updateMesh();
}

void World::generateChunk(Chunk* chunk) {
    PROFILE_FUNCTION();
    const ChunkData generatedChunkData = generator->generate(chunk->getPosition() * Constants::ChunkSize);
    chunk->setData(generatedChunkData);
    for (const auto& direction : DIRECTIONS) {
        const glm::ivec3 neighborPos = chunk->getPosition() + getNormal(direction);
        Chunk* neighbor = chunkManager.getChunkAt(neighborPos);
        if (!neighbor) continue;
        neighbor->markDirty();
        chunk->setNeighbor(direction, neighbor);
        neighbor->setNeighbor(getOpposite(direction), chunk);
    }
}

void World::generateFromPlayerPosition(const glm::ivec3& position) {
    const auto chunkPosition = glm::ivec3(
        floorDiv(position.x, Constants::ChunkSize),
        floorDiv(position.y, Constants::ChunkSize),
        floorDiv(position.z, Constants::ChunkSize)
    );
    generateFromChunkPosition(chunkPosition);
}

void World::generateFromChunkPosition(const glm::ivec3 playerPosition) {
    PROFILE_FUNCTION();

    static int nbChunkLoaded = 0;
    static std::optional<glm::ivec3> lastPosition;

    if (lastPosition && *lastPosition == playerPosition) return;
    lastPosition = playerPosition;

    for (int x = playerPosition.x - Constants::RenderDistance; x <= playerPosition.x + Constants::RenderDistance; x++) {
        for (int y = - Constants::RenderHeight; y < Constants::RenderHeight; y++) {
            for (int z = playerPosition.z - Constants::RenderDistance; z <= playerPosition.z + Constants::RenderDistance; z++) {
                const int dx = x - playerPosition.x;
                const int dz = z - playerPosition.z;
                if (!chunkManager.hasChunkAt({x, y, z}) && dx * dx + dz * dz <= Constants::RenderDistance * Constants::RenderDistance) {
                    auto chunk = std::make_unique<Chunk>(glm::ivec3(x, y, z)); // create
                    generateChunk(chunk.get()); // generate
                    chunkManager.addChunk(std::move(chunk)); // pointer move

                    nbChunkLoaded++;
                }
            }
        }
    }

    std::vector<glm::ivec3> chunksToRemove;

    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) {
        glm::ivec3 cPos = chunkPtr->getPosition();
        const int dx = cPos.x - playerPosition.x;
        const int dz = cPos.z - playerPosition.z;

        if (dx * dx + dz * dz > Constants::RenderDistance * Constants::RenderDistance ||
            cPos.y < -Constants::RenderHeight || cPos.y >= Constants::RenderHeight) {
            chunksToRemove.push_back(cPos);
        }
    }

    for (const auto& pos : chunksToRemove) {
        chunkManager.removeChunk(pos);
        nbChunkLoaded--;
    }

    std::cout << "Chunks loaded: " << nbChunkLoaded
          << " | Memory usage: " << nbChunkLoaded * 0.015625 << " MB" << std::endl;

}
