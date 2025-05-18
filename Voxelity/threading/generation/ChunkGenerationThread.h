//
// Created by adrian on 18/05/25.
//

#ifndef CHUNKGENERATIONTHREAD_H
#define CHUNKGENERATIONTHREAD_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <GLT.h>

class IWorldGenerator;
struct ChunkData;

class ChunkGenerationThread {
public:
    explicit ChunkGenerationThread(std::unique_ptr<IWorldGenerator> generator);
    ~ChunkGenerationThread();

    void start();
    void stop();

    void enqueuePosition(const glm::ivec3& pos);

    bool pollReadyChunk(glm::ivec3& posOut, ChunkData& dataOut);
private:
    void run();

    std::thread workerThread;
    std::atomic<bool> running = false;

    std::queue<glm::ivec3> positionsToGenerate;
    std::mutex queueMutex;
    std::condition_variable cv;

    std::queue<std::pair<glm::ivec3, ChunkData>> readyChunks;
    std::mutex readyMutex;

    std::unique_ptr<IWorldGenerator> generator;
};

#endif //CHUNKGENERATIONTHREAD_H
