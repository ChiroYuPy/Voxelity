//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <ranges>
#include <utility>

#include "chunk/Chunk.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "math/Frustum.h"
#include "meshers/IChunkMesher.h"

inline int floorDiv(const int a, const int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

// TODO: DONE   Lazy Update ( dirty flag )
// TODO: DONE   Distance Culling ( Render Distance )
// TODO: DONE   Frustum Culling ( viewProj & chunk AABB )
// TODO: DONE   Face Culling ( isVisible() test )
// TODO: DONE   Basic Occlusion Culling ( isEmpty() & hasVisibleFaces() tests )
// TODO         Complete Occlusion Culling
// TODO         Level Of Details ( 16³, 8³, 4³, 2³, 1 chunk mesh volumes related to the distance )
// TODO         Greedy Meshing
// TODO         Multi-Threading -> world generation, chunk meshing, rendering

// TODO: DONE   VoxelFace Data Compression

// TODO         Minimize Shaders Calculs
// TODO         Optimize Shaders Uniforms

World::World(std::unique_ptr<IChunkMesher> mesher, std::unique_ptr<IWorldGenerator> generator)
: mesher(std::move(mesher)), generationThread(std::move(generator)) {
    generationThread.start();
}

void World::render(const glm::vec3& cameraPosition,
                   const glm::mat4& view,
                   const glm::mat4& projection,
                   const glm::vec3& lightDirection,
                   const glm::vec3& lightColor,
                   const glm::vec3& ambientColor) {
    chunkRenderer.render(chunkManager, cameraPosition, view, projection, lightDirection, lightColor, ambientColor);
}

void World::updateMeshes() {
    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) {
        if (chunkPtr->isDirty() or chunkPtr->getState() == ChunkState::Generated) {
            const std::vector<VoxelFace> voxelFaces = mesher->mesh(*chunkPtr);
            ChunkMesh& chunkMesh = chunkPtr->getMesh();
            chunkMesh.setVoxelFaces(voxelFaces);
            chunkMesh.build();
            chunkPtr->setState(ChunkState::ReadyToRender);
        }
    }
}

void World::update() {
    updateMeshes();

    glm::ivec3 pos;
    ChunkData data;
    while (generationThread.pollReadyChunk(pos, data)) {
        if (!chunkManager.hasChunkAt(pos)) continue;
        const auto chunk = chunkManager.getChunkAt(pos);
        chunk->setData(data);
        chunk->setState(ChunkState::Generated);

        for (const auto& dir : DIRECTIONS) {
            const glm::ivec3 neighborPos = pos + getNormal(dir);
            Chunk* neighbor = chunkManager.getChunkAt(neighborPos);
            if (neighbor) {
                chunk->setNeighbor(dir, neighbor);
                neighbor->setNeighbor(getOpposite(dir), chunk);
                neighbor->setState(ChunkState::MeshDirty);
            }
        }
    }
}

void World::updateFromPlayerPosition(const glm::ivec3& playerWorldPos) {
    const glm::ivec3 chunkPos = {
        floorDiv(playerWorldPos.x, Constants::ChunkSize),
        floorDiv(playerWorldPos.y, Constants::ChunkSize),
        floorDiv(playerWorldPos.z, Constants::ChunkSize)
    };
    chunkLoader.updateChunksAround(chunkPos, chunkManager, generationThread);
}