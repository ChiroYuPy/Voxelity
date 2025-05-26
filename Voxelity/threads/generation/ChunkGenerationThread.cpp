//
// Created by adrian on 18/05/25.
//

#include "ChunkGenerationThread.h"

#include "core/Constants.h"
#include "../../voxelWorld/chunk/VoxelStorage.h"
#include "voxelWorld/generators/IWorldGenerator.h"

ChunkGenerationThread::ChunkGenerationThread(std::unique_ptr<IChunkGenerator> generator)
: chunkGenerator(std::move(generator)) {}

ChunkGenerationThread::~ChunkGenerationThread() {
    stop();
}

void ChunkGenerationThread::start() {
    running = true;
    workerThread = std::thread(&ChunkGenerationThread::run, this);
}

void ChunkGenerationThread::stop() {
    {
        std::lock_guard lock(queueMutex);
        running = false;
        cv.notify_all();
    }
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ChunkGenerationThread::enqueueElement(const glm::ivec3& pos) {
    {
        std::lock_guard lock(queueMutex);
        queueElements.push(pos);
    }
    cv.notify_one();
}

bool ChunkGenerationThread::pollReadyElements(glm::ivec3& posOut, VoxelStorage& dataOut) {
    std::lock_guard lock(readyMutex);
    if (readyElements.empty()) return false;

    auto [pos, data] = std::move(readyElements.front());
    readyElements.pop();

    posOut = pos;
    dataOut = data;
    return true;
}

void ChunkGenerationThread::run() {
    while (running) {
        glm::ivec3 pos;

        {
            std::unique_lock lock(queueMutex);
            cv.wait(lock, [&] { return !queueElements.empty() || !running; });
            if (!running) break;

            pos = queueElements.front();
            queueElements.pop();
        }

        // Génération

        {
            VoxelStorage data = chunkGenerator->generate(pos * Constants::ChunkSize);
            std::lock_guard readyLock(readyMutex);
            readyElements.emplace(pos, data);
        }
    }
}
