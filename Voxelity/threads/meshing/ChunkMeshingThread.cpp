//
// Created by adrian on 18/05/25.
//

#include "ChunkMeshingThread.h"

#include "voxelWorld/chunk/VoxelStorage.h"
#include "voxelWorld/chunk/ChunkMeshData.h"
#include "voxelWorld/meshBuilders/IChunkMeshBuilder.h"
#include "voxelWorld/meshBuilders/ChunkDataNeighborhood.h"

ChunkMeshingThread::ChunkMeshingThread(std::unique_ptr<IChunkMeshBuilder> meshBuilder)
: meshBuilder(std::move(meshBuilder)) {}

ChunkMeshingThread::~ChunkMeshingThread() {
    stop();
}

void ChunkMeshingThread::start() {
    running = true;
    workerThread = std::thread(&ChunkMeshingThread::run, this);
}

void ChunkMeshingThread::stop() {
    {
        std::lock_guard lock(queueMutex);
        running = false;
        cv.notify_all();
    }
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ChunkMeshingThread::enqueueElement(const glm::ivec3& pos, const ChunkDataNeighborhood& chunkDataNeighborhood) {
    {
        std::lock_guard guard(queueMutex);
        queueElements.emplace(pos, chunkDataNeighborhood);
    }
    cv.notify_one();
}

bool ChunkMeshingThread::pollReadyElements(glm::ivec3& posOut, ChunkMeshData& chunkMeshDataOut) {
    std::lock_guard lock(readyMutex);
    if (readyElements.empty()) return false;

    auto [pos, meshData] = std::move(readyElements.front());
    readyElements.pop();

    posOut = pos;
    chunkMeshDataOut = std::move(meshData);
    return true;
}

void ChunkMeshingThread::run() {
    while (running) {
        std::pair<glm::ivec3, ChunkDataNeighborhood> task;

        {
            std::unique_lock lock(queueMutex);
            cv.wait(lock, [&] { return !queueElements.empty() || !running; });

            if (!running && queueElements.empty()) break;

            task = std::move(queueElements.front());
            queueElements.pop();
        }

        const glm::ivec3& pos = task.first;
        const ChunkDataNeighborhood& neighborhood = task.second;

        // Mesh building

        const std::vector<VoxelFace> voxelFaces = meshBuilder->mesh(neighborhood);
        ChunkMeshData meshData;
        meshData.setVoxelFaces(voxelFaces);

        {
            std::lock_guard guard(readyMutex);
            readyElements.emplace(pos, std::move(meshData));
        }
    }
}
