//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <iostream>
#include <ranges>
#include <utility>

#include "chunk/Chunk.h"
#include "chunk/ChunkMeshData.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "generators/NaturaldGenerator.h"
#include "math/Frustum.h"
#include "meshBuilders/ChunkDataNeighborhood.h"
#include "meshBuilders/IChunkMeshBuilder.h"
#include "threads/meshing/ChunkMeshingThread.h"

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

    worldChunkData = std::make_unique<WorldChunkData>();
    chunkRenderer = std::make_unique<WorldChunkRenderer>();
    chunkGenerationRequestManager = std::make_unique<ChunkGenerationRequestManager>(std::move(generator));

    chunkMeshingThread = std::make_unique<ChunkMeshingThread>(std::move(meshBuilder_));
    chunkMeshingThread->start();
}

World::~World() {
    chunkMeshingThread->stop();
}

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
    enqueueDirtyChunks();
    processReadyMeshes();
}

void World::enqueueDirtyChunks() const {
    for (const auto &chunkPtr: worldChunkData->chunks | std::views::values) {
        if (!chunkPtr) continue;
        if (chunkPtr->getState() == ChunkState::Generated || chunkPtr->isDirty()) {

            // Prepare neighborhood
            std::array<Chunk*,6> neighbors = chunkPtr->getNeighbors();
            std::array<ChunkData*,6> dataNbrs{};
            for (int i = 0; i < 6; ++i)
                dataNbrs[i] = neighbors[i] ? &neighbors[i]->getData() : nullptr;

            ChunkDataNeighborhood neighborhood{&chunkPtr->getData(), dataNbrs};
            chunkMeshingThread->enqueueElement(chunkPtr->getPosition(), neighborhood);
            // mark as meshing
            std::cout << "enqueueing" << std::endl;
            chunkPtr->setState(ChunkState::Meshing);
        }
    }
}

void World::processReadyMeshes() const {
    glm::ivec3 pos;
    ChunkMeshData meshData;

    while (chunkMeshingThread->pollReadyElements(pos, meshData)) {
        if (!worldChunkData->hasChunkAt(pos)) continue;
        Chunk* chunkPtr = worldChunkData->getChunkAt(pos);
        if (!chunkPtr) continue;

        if (chunkPtr->getState() != ChunkState::Meshing) continue;

        chunkPtr->getMesh().upload(meshData);
        chunkPtr->setState(ChunkState::ReadyToRender);
    }
}

void World::updateFromPlayerPosition(const glm::ivec3& playerWorldPos) const {
    const glm::ivec3 chunkPos = {
        floorDiv(playerWorldPos.x, Constants::ChunkSize),
        floorDiv(playerWorldPos.y, Constants::ChunkSize),
        floorDiv(playerWorldPos.z, Constants::ChunkSize)
    };

    chunkGenerationRequestManager->updateChunksAround(chunkPos, *worldChunkData);
}
