//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "rendering/Mesh.h"
#include "rendering/Shader.h"
#include "rendering/VoxelFace.h"
#include "voxelWorld/Chunk.h"
#include "voxelWorld/generators/FlatWorldGenerator.h"
#include "voxelWorld/generators/NaturalWorldGenerator.h"

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
    std::cout << i << std::endl;
}

void World::update() const {
    for (const auto &val: chunks | std::views::values) val->updateMesh();
}

unsigned long World::chunkKey(const int cx, const int cy, const int cz) {
    constexpr int OFFSET = 1 << 20;

    const unsigned long ux = static_cast<unsigned long>(cx + OFFSET) & 0x1FFFFF;
    const unsigned long uy = static_cast<unsigned long>(cy + OFFSET) & 0x1FFFFF;
    const unsigned long uz = static_cast<unsigned long>(cz + OFFSET) & 0x1FFFFF;

    return (ux << 42) | (uy << 21) | uz;
}

void World::generate(int cx, int cy, int cz) {
    const auto key = chunkKey(cx, cy, cz);
    if (!chunks.contains(key)) {
        auto chunk = std::make_unique<Chunk>(cx, cy, cz);
        generator->generate(*chunk);
        chunks[key] = std::move(chunk);
    }
}

void World::generateFromPosition(glm::ivec3 position) {
    static constexpr int RENDER_DISTANCE = 16;
    static constexpr int RENDER_HEIGHT = 2;

    position /= CHUNK_SIZE;

    const unsigned long ck = chunkKey(position.x, position.y, position.z);
    if (!chunks.contains(ck)) {
        for (int x = position.x - RENDER_DISTANCE; x < position.x + RENDER_DISTANCE; x++) {
            for (int y = - RENDER_HEIGHT; y < RENDER_HEIGHT; y++) {
                for (int z = position.z - RENDER_DISTANCE; z < position.z + RENDER_DISTANCE; z++) {
                    float distance = std::sqrt((x - position.x) * (x - position.x) + (z - position.z) * (z - position.z));
                    if (distance <= RENDER_DISTANCE) {
                        generate(x, y, z);
                    }
                }
            }
        }
    }
}
