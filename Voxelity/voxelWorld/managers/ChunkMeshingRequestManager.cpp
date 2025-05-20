//
// Created by adrian on 20/05/25.
//

#include "ChunkMeshingRequestManager.h"

#include <array>
#include <ranges>

#include "voxelWorld/WorldChunkData.h"
#include "voxelWorld/chunk/Chunk.h"
#include "voxelWorld/chunk/ChunkMeshData.h"
#include "voxelWorld/meshBuilders/ChunkDataNeighborhood.h"
#include "voxelWorld/meshBuilders/IChunkMeshBuilder.h"

struct ChunkData;
class Chunk;

ChunkMeshingRequestManager::ChunkMeshingRequestManager(std::unique_ptr<IChunkMeshBuilder> meshBuilder) {
    chunkMeshingThread = std::make_unique<ChunkMeshingThread>(std::move(meshBuilder));
    chunkMeshingThread->start();
}

ChunkMeshingRequestManager::~ChunkMeshingRequestManager() {
    chunkMeshingThread->stop();
}

void ChunkMeshingRequestManager::enqueueDirtyChunks(WorldChunkData& worldChunkData) const {
    for (const auto& chunkPtr : worldChunkData.chunks | std::views::values) {
        if (!chunkPtr) continue;
        if (!chunkPtr->isDirty()) continue;

        std::array<Chunk*, 6> neighbors = chunkPtr->getNeighbors();
        std::array<ChunkData*, 6> dataNbrs{};
        for (int i = 0; i < 6; ++i)
            dataNbrs[i] = neighbors[i] ? &neighbors[i]->getData() : nullptr;

        ChunkDataNeighborhood neighborhood{&chunkPtr->getData(), dataNbrs};
        chunkMeshingThread->enqueueElement(chunkPtr->getPosition(), neighborhood);
        chunkPtr->setState(ChunkState::Meshing);
    }
}

void ChunkMeshingRequestManager::processReadyMeshes(WorldChunkData& worldChunkData) const {
    glm::ivec3 pos;
    ChunkMeshData meshData;

    while (chunkMeshingThread->pollReadyElements(pos, meshData)) {
        if (!worldChunkData.hasChunkAt(pos)) continue;
        Chunk* chunkPtr = worldChunkData.getChunkAt(pos);
        if (!chunkPtr || chunkPtr->getState() != ChunkState::Meshing) continue;

        chunkPtr->getMesh().upload(meshData);
        chunkPtr->setState(ChunkState::ReadyToRender);
    }
}
