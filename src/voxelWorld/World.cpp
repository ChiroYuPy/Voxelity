//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "rendering/Mesh.h"
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
}

void World::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDirection) const {
    chunkShader->use();
    chunkShader->setUniform("uView", view);
    chunkShader->setUniform("uProjection", projection);
    chunkShader->setUniform("uLightDirection", lightDirection);

    int i = 0;
    for (const auto &val: chunks | std::views::values) {
        val->getMesh()->render();
        i++;
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

void World::generate(const int cx, const int cy, const int cz) {
    const auto key = chunkKey(cx, cy, cz);
    if (!chunks.contains(key)) {
        auto chunk = std::make_unique<Chunk>(cx, cy, cz, this);
        generator->generate(*chunk);
        chunks[key] = std::move(chunk);

        static constexpr glm::ivec3 DIRECTIONS[6] = {
            {1, 0, 0}, {-1, 0, 0},
            {0, 1, 0}, {0, -1, 0},
            {0, 0, 1}, {0, 0, -1},
        };

        for (const auto& dir : DIRECTIONS) {
            Chunk* neighbor = getChunkAt(cx + dir.x, cy + dir.y, cz + dir.z);
            if (neighbor) neighbor->setDirty(true);
        }
    }
}

void World::generateFromPosition(const glm::ivec3 position) {
    static constexpr int RENDER_DISTANCE = 8;
    static constexpr int RENDER_HEIGHT = 1;

    const auto chunkPos = glm::ivec3(
        floorDiv(position.x, CHUNK_SIZE),
        floorDiv(position.y, CHUNK_SIZE),
        floorDiv(position.z, CHUNK_SIZE)
    );

    const unsigned long ck = chunkKey(chunkPos.x, chunkPos.y, chunkPos.z);
    if (!chunks.contains(ck)) {
        for (int x = chunkPos.x - RENDER_DISTANCE; x <= chunkPos.x + RENDER_DISTANCE; x++) {
            for (int y = - RENDER_HEIGHT; y <= RENDER_HEIGHT; y++) {
                for (int z = chunkPos.z - RENDER_DISTANCE; z <= chunkPos.z + RENDER_DISTANCE; z++) {
                    const int dx = x - chunkPos.x;
                    const int dz = z - chunkPos.z;
                    if (dx * dx + dz * dz <= RENDER_DISTANCE * RENDER_DISTANCE) generate(x, y, z);
                }
            }
        }
    }
}
