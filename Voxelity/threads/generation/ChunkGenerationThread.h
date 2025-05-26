//
// Created by adrian on 18/05/25.
//

#ifndef CHUNKGENERATIONTHREAD_H
#define CHUNKGENERATIONTHREAD_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <GLT.h>

class IChunkGenerator;
struct VoxelStorage;

class ChunkGenerationThread {
public:
    explicit ChunkGenerationThread(std::unique_ptr<IChunkGenerator> chunkGenerator);
    ~ChunkGenerationThread();

    void start();
    void stop();

    void enqueueElement(const glm::ivec3& pos);

    bool pollReadyElements(glm::ivec3& posOut, VoxelStorage& dataOut);
private:
    void run();

    std::thread workerThread;
    std::atomic<bool> running = false;

    std::queue<glm::ivec3> queueElements;
    std::mutex queueMutex;
    std::condition_variable cv;

    std::queue<std::pair<glm::ivec3, VoxelStorage>> readyElements;
    std::mutex readyMutex;

    std::unique_ptr<IChunkGenerator> chunkGenerator;
};

#endif //CHUNKGENERATIONTHREAD_H
