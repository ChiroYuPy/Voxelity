//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "engine/Application.h"
#include "core/utils/Profiler.h"
#include "math/Frustum.h"
#include "rendering/ChunkMesh.h"
#include "rendering/shader/Shader.h"
#include "voxelWorld/generators/FlatWorldGenerator.h"
#include "voxelWorld/generators/NaturalWorldGenerator.h"

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

World::World() {
    generator = std::make_unique<NaturalWorldGenerator>();

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

    for (const auto& chunkPtr : chunks | std::views::values) {
        const auto& chunk = *chunkPtr;
        // AABB calcul
        const glm::vec3 chunkWorldMin = chunk.getPosition() * Chunk::SIZE; // GOULET ?
        constexpr glm::vec3 halfSize = glm::vec3(Chunk::SIZE) * 0.5f;
        const glm::vec3 center = chunkWorldMin + halfSize;

        // frustum culling
        if (frustum.intersectsAABB(center, halfSize) && chunk.getMesh()->hasVisibleFaces()) {
            chunkShader->setUniform("uChunkOffset", chunk.getWorldPosition());
            chunk.getMesh()->render();
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
    for (const auto &val: chunks | std::views::values) val->updateMesh();
}

Chunk* World::getChunkAt(const int cx, const int cy, const int cz) const {
    const auto key = chunkKey(cx, cy, cz);
    const auto it = chunks.find(key);
    return it != chunks.end() ? it->second.get() : nullptr;
}

const Voxel* World::getVoxelAt(const int vx, const int vy, const int vz) const {
    const int cx = floorDiv(vx, Chunk::SIZE);
    const int cy = floorDiv(vy, Chunk::SIZE);
    const int cz = floorDiv(vz, Chunk::SIZE);

    const int xInChunk = ((vx % Chunk::SIZE) + Chunk::SIZE) % Chunk::SIZE;
    const int yInChunk = ((vy % Chunk::SIZE) + Chunk::SIZE) % Chunk::SIZE;
    const int zInChunk = ((vz % Chunk::SIZE) + Chunk::SIZE) % Chunk::SIZE;

    const Chunk* chunk = getChunkAt(cx, cy, cz);
    if (!chunk) return nullptr;

    return chunk->at(xInChunk, yInChunk, zInChunk);
}

unsigned long World::chunkKey(const int cx, const int cy, const int cz) {
    constexpr int OFFSET = 1 << 20;

    const unsigned long ux = static_cast<unsigned long>(cx + OFFSET) & 0x1FFFFF;
    const unsigned long uy = static_cast<unsigned long>(cy + OFFSET) & 0x1FFFFF;
    const unsigned long uz = static_cast<unsigned long>(cz + OFFSET) & 0x1FFFFF;

    return (ux << 42) | (uy << 21) | uz;
}

void World::generate(const int cx, const int cy, const int cz) {
    static int counter = 0;
    counter++;
    std::cout << "chunks loaded: " << counter << " | voxels loaded: " << counter * 4096 <<std::endl;
    const auto key = chunkKey(cx, cy, cz);
    if (!chunks.contains(key)) {
        auto chunk = std::make_unique<Chunk>(glm::ivec3(cx, cy, cz));
        generateChunk(chunk.get());
        chunks[key] = std::move(chunk);
    }
}

void World::generateChunk(Chunk* chunk) const {
    PROFILE_FUNCTION();
    generator->generate(*chunk);
    for (const auto& direction : DIRECTIONS) {
        const glm::vec3 neighborPos = chunk->getPosition() + getNormal(direction);
        Chunk* neighbor = getChunkAt(static_cast<int>(neighborPos.x), static_cast<int>(neighborPos.y), static_cast<int>(neighborPos.z));
        if (!neighbor) continue;
        neighbor->markDirty();
        chunk->setNeighbor(direction, neighbor);
        neighbor->setNeighbor(getOpposite(direction), chunk);
    }
}

void World::generateFromPosition(const glm::ivec3 position) {
    PROFILE_FUNCTION();

    const auto chunkPos = glm::ivec3(
        floorDiv(position.x, Chunk::SIZE),
        floorDiv(position.y, Chunk::SIZE),
        floorDiv(position.z, Chunk::SIZE)
    );

    for (int x = chunkPos.x - RENDER_DISTANCE; x <= chunkPos.x + RENDER_DISTANCE; x++) {
        for (int y = - CHUNK_RENDER_HEIGHT; y < CHUNK_RENDER_HEIGHT; y++) {
            for (int z = chunkPos.z - RENDER_DISTANCE; z <= chunkPos.z + RENDER_DISTANCE; z++) {
                const int dx = x - chunkPos.x;
                const int dz = z - chunkPos.z;
                if (dx * dx + dz * dz <= RENDER_DISTANCE * RENDER_DISTANCE) generate(x, y, z);
            }
        }
    }

    std::vector<glm::ivec3> chunksToRemove;

    for (const auto &chunk: chunks | std::views::values) {
        glm::ivec3 cPos = chunk->getPosition();
        const int dx = cPos.x - chunkPos.x;
        const int dz = cPos.z - chunkPos.z;

        if (dx * dx + dz * dz > RENDER_DISTANCE * RENDER_DISTANCE ||
            cPos.y < -CHUNK_RENDER_HEIGHT || cPos.y >= CHUNK_RENDER_HEIGHT) {
            chunksToRemove.push_back(cPos);
            }
    }

    // Supprimer les chunks hors zone de rendu
    for (const auto& pos : chunksToRemove) {
        const auto key = chunkKey(pos.x, pos.y, pos.z);
        chunks.erase(key);
    }
}
