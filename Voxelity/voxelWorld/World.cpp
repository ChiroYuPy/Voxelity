//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/World.h"

#include <ranges>
#include <utility>

#include "chunk/Chunk.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "generators/NaturalWorldGenerator.h"
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

World::World(std::unique_ptr<IChunkMeshBuilder> mesher, std::unique_ptr<IWorldGenerator> generator) {
    meshBuilder = std::move(mesher);

    chunkData = std::make_unique<WorldChunkData>();
    chunkRenderer = std::make_unique<WorldChunkRenderer>();
    chunkLoader = std::make_unique<ChunkGenerationRequestManager>(std::move(generator));

}

World::~World() = default;

void World::render(const glm::vec3& cameraPosition,
                   const glm::mat4& view,
                   const glm::mat4& projection,
                   const glm::vec3& lightDirection,
                   const glm::vec3& lightColor,
                   const glm::vec3& ambientColor) const {
    chunkRenderer->render(*chunkData, cameraPosition, view, projection, lightDirection, lightColor, ambientColor);
}

void World::updateMeshes() const {
    for (const auto &chunkPtr: chunkData->chunks | std::views::values) {
        if (chunkPtr->isDirty() or chunkPtr->getState() == ChunkState::Generated) {
            const std::vector<VoxelFace> voxelFaces = meshBuilder->mesh(*chunkPtr);
            ChunkMesh& chunkMesh = chunkPtr->getMesh();
            chunkMesh.setVoxelFaces(voxelFaces);
            chunkMesh.build();
            chunkPtr->setState(ChunkState::ReadyToRender);
        }
    }
}

void World::update() const {
    chunkLoader->processReadyChunks(*chunkData);
    updateMeshes();
}

void World::updateFromPlayerPosition(const glm::ivec3& playerWorldPos) const {
    const glm::ivec3 chunkPos = {
        floorDiv(playerWorldPos.x, Constants::ChunkSize),
        floorDiv(playerWorldPos.y, Constants::ChunkSize),
        floorDiv(playerWorldPos.z, Constants::ChunkSize)
    };

    chunkLoader->updateChunksAround(chunkPos, *chunkData);
}
