//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <utility>

#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "generators/NaturaldGenerator.h"
#include "managers/ChunkMeshingRequestManager.h"
#include "math/Frustum.h"
#include "meshBuilders/IChunkMeshBuilder.h"

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

World::World(std::unique_ptr<IChunkMeshBuilder> meshBuilder_, std::unique_ptr<IChunkGenerator> generator) {

    worldChunkData = std::make_unique<ChunkStorage>();
    chunkRenderer = std::make_unique<WorldChunkRenderer>();
    chunkGenerationRequestManager = std::make_unique<ChunkGenerationRequestManager>(std::move(generator));
    chunkMeshingRequestManager = std::make_unique<ChunkMeshingRequestManager>(std::move(meshBuilder_));
}

World::~World() = default;

void World::render(const glm::vec3& cameraPosition,
                   const glm::mat4& view,
                   const glm::mat4& projection,
                   const glm::vec3& lightDirection,
                   const glm::vec3& lightColor,
                   const glm::vec3& ambientColor) const {
    chunkRenderer->render(*worldChunkData, cameraPosition, view, projection, lightDirection, lightColor, ambientColor);
}

void World::update() const {
    chunkGenerationRequestManager->processReadyChunks(*worldChunkData);

    chunkMeshingRequestManager->enqueueDirtyChunks(*worldChunkData);
    chunkMeshingRequestManager->processReadyMeshes(*worldChunkData);
}

ChunkStorage& World::getWorldChunkData() const {
    return *worldChunkData;
}

void World::updateFromPlayerPosition(const glm::ivec3& playerWorldPos) const {
    const glm::ivec3 chunkPos = {
        floorDiv(playerWorldPos.x, Constants::CHUNK_SIZE),
        floorDiv(playerWorldPos.y, Constants::CHUNK_SIZE),
        floorDiv(playerWorldPos.z, Constants::CHUNK_SIZE)
    };

    chunkGenerationRequestManager->updateChunksAround(chunkPos, *worldChunkData);
}
