//
// Created by adrian on 18/05/25.
//

#include "ChunkGenerationThread.h"

#include "core/Constants.h"
#include "voxelWorld/chunk/ChunkData.h"
#include "voxelWorld/generators/IWorldGenerator.h"

ChunkGenerationThread::ChunkGenerationThread(std::unique_ptr<IWorldGenerator> generator)
    : generator(std::move(generator)) {}

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

void ChunkGenerationThread::enqueuePosition(const glm::ivec3& pos) {
    {
        std::lock_guard lock(queueMutex);
        positionsToGenerate.push(pos);
    }
    cv.notify_one();
}

bool ChunkGenerationThread::pollReadyChunk(glm::ivec3& posOut, ChunkData& dataOut) {
    std::lock_guard lock(readyMutex);
    if (readyChunks.empty()) return false;

    auto [pos, data] = std::move(readyChunks.front());
    readyChunks.pop();

    posOut = pos;
    dataOut = data;
    return true;
}

void ChunkGenerationThread::run() {
    while (running) {
        glm::ivec3 pos;

        {
            std::unique_lock lock(queueMutex);
            cv.wait(lock, [&] { return !positionsToGenerate.empty() || !running; });
            if (!running) break;

            pos = positionsToGenerate.front();
            positionsToGenerate.pop();
        }

        // Génération

        {
            ChunkData data = generator->generate(pos * Constants::ChunkSize);
            std::lock_guard readyLock(readyMutex);
            readyChunks.emplace(pos, data);
        }
    }
}
