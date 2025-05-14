//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <ranges>

#include "rendering/VoxelFace.h"
#include "voxelWorld/generators/FlatWorldGenerator.h"
#include "voxelWorld/generators/NaturalWorldGenerator.h"

World::World() {
    generator = std::make_unique<NaturalWorldGenerator>();
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

void World::generateFromPosition(const int fx, const int fy, const int fz) {
    static constexpr int RENDER_DISTANCE = 8;
    static constexpr int RENDER_HEIGHT = 2;

    for (int x = fx - RENDER_DISTANCE; x < fx + RENDER_DISTANCE; x++) {
        for (int y = fy - RENDER_HEIGHT; y < fy + RENDER_HEIGHT; y++) {
            for (int z = fz - RENDER_DISTANCE; z < fz + RENDER_DISTANCE; z++) {
                float distance = std::sqrt((x - fx) * (x - fx) + (z - fz) * (z - fz));
                if (distance <= RENDER_DISTANCE && y >= fy - RENDER_HEIGHT && y <= fy + RENDER_HEIGHT) {
                    generate(x, y, z);
                }
            }
        }
    }
}

bool World::isFaceVisible(const int x, const int y, const int z, const Chunk& chunk, const int face) {
    // Pour simplifier, on ne regarde que dans ce chunk
    static const glm::ivec3 directions[6] = {
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0, -1, 0},
        {0, 0, 1}, {0, 0, -1}
    };
    const glm::ivec3 offset = directions[face];
    const int nx = x + offset.x;
    const int ny = y + offset.y;
    const int nz = z + offset.z;

    if (nx < 0 || ny < 0 || nz < 0 || nx >= CHUNK_SIZE || ny >= CHUNK_SIZE || nz >= CHUNK_SIZE)
        return true; // bord du chunk = visible

    return !chunk.get(nx, ny, nz).isSolid();
}

std::vector<VoxelFace> World::generateFaceInstances() const {
    std::vector<VoxelFace> faces;

    for (const auto &chunkPtr: chunks | std::views::values) {
        const auto& chunk = *chunkPtr;
        for (int z = 0; z < CHUNK_SIZE; ++z)
        for (int y = 0; y < CHUNK_SIZE; ++y)
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            Voxel voxel = chunk.get(x, y, z);
            if (!voxel.isSolid()) continue;

            const glm::ivec3 worldPos = chunk.getPosition() * CHUNK_SIZE + glm::ivec3(x, y, z);
            for (int face = 0; face < 6; ++face) {
                if (isFaceVisible(x, y, z, chunk, face)) {
                    faces.emplace_back(
                        worldPos,
                        face,
                        static_cast<int>(voxel.type)
                    );
                }
            }
        }
    }

    return faces;
}
