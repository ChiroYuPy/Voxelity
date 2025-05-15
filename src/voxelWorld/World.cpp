//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Profiler.h"
#include "rendering/ChunkMesh.h"
#include "rendering/Shader.h"
#include "voxelWorld/generators/FlatWorldGenerator.h"
#include "voxelWorld/generators/NaturalWorldGenerator.h"

inline int floorDiv(const int a, const int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

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
                   const glm::vec3& ambientColor) const {
    PROFILE_FUNCTION();
    chunkShader->use();

    chunkShader->setUniform("uView", view);
    chunkShader->setUniform("uProjection", projection);

    chunkShader->setUniform("uLightDirection", glm::normalize(lightDirection));
    chunkShader->setUniform("uLightColor", lightColor);

    chunkShader->setUniform("uAmbientColor", ambientColor);

    glActiveTexture(GL_TEXTURE0);
    textureAtlas->bind(0);
    chunkShader->setUniform("uAtlas", 0);

    // Render de tous les chunks
    for (const auto &chunk: chunks | std::views::values) {
        chunk->getMesh()->render();
    }
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
    const int cx = floorDiv(vx, CHUNK_SIZE);
    const int cy = floorDiv(vy, CHUNK_SIZE);
    const int cz = floorDiv(vz, CHUNK_SIZE);

    const int xInChunk = ((vx % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    const int yInChunk = ((vy % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    const int zInChunk = ((vz % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;

    const Chunk* chunk = getChunkAt(cx, cy, cz);
    if (!chunk) return nullptr;

    return chunk->getVoxelAt(xInChunk, yInChunk, zInChunk);
}

unsigned long World::chunkKey(const int cx, const int cy, const int cz) {
    constexpr int OFFSET = 1 << 20;

    const unsigned long ux = static_cast<unsigned long>(cx + OFFSET) & 0x1FFFFF;
    const unsigned long uy = static_cast<unsigned long>(cy + OFFSET) & 0x1FFFFF;
    const unsigned long uz = static_cast<unsigned long>(cz + OFFSET) & 0x1FFFFF;

    return (ux << 42) | (uy << 21) | uz;
}

static constexpr glm::ivec3 DIRECTIONS[6] = {
    {1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1},
};

void World::generate(const int cx, const int cy, const int cz) {
    static int counter = 0;
    const auto key = chunkKey(cx, cy, cz);
    if (!chunks.contains(key)) {
        counter++;
        std::cout << counter << std::endl;

        auto chunk = std::make_unique<Chunk>(cx, cy, cz, this);
        generator->generate(*chunk);
        chunks[key] = std::move(chunk);
        for (const auto& dir : DIRECTIONS) {
            Chunk* neighbor = getChunkAt(cx + dir.x, cy + dir.y, cz + dir.z);
            if (neighbor) neighbor->setDirty(true);
        }
    }
}

void World::generateFromPosition(const glm::ivec3 position) {
    PROFILE_FUNCTION();
    static constexpr int RENDER_DISTANCE = 24;
    static constexpr int CHUNK_RENDER_HEIGHT = 4;

    const auto chunkPos = glm::ivec3(
        floorDiv(position.x, CHUNK_SIZE),
        floorDiv(position.y, CHUNK_SIZE),
        floorDiv(position.z, CHUNK_SIZE)
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

    for (const auto& [key, chunk] : chunks) {
        glm::ivec3 cPos = chunk->getPosition();
        int dx = cPos.x - chunkPos.x;
        int dz = cPos.z - chunkPos.z;

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
