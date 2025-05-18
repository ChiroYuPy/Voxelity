//
// Created by adrian on 18/05/25.
//

#ifndef CHUNKMESHINGTHREAD_H
#define CHUNKMESHINGTHREAD_H

#include <condition_variable>
#include <memory>
#include <GLT.h>
#include <queue>

struct ChunkDataNeighborhood;
class ChunkMeshData;
class IChunkMeshBuilder;

class ChunkMeshingThread {
public:
    explicit ChunkMeshingThread(std::unique_ptr<IChunkMeshBuilder> meshBuilder);
    ~ChunkMeshingThread();

    void start();
    void stop();

    void enqueueElement(const glm::ivec3& pos, const ChunkDataNeighborhood& chunkDataNeighborhood);

    bool pollReadyElements(glm::ivec3& posOut, ChunkMeshData& chunkMeshDataOut);
private:
    void run();

    std::thread workerThread;
    std::atomic<bool> running = false;

    std::queue<std::pair<glm::ivec3, ChunkDataNeighborhood>> queueElements;
    std::mutex queueMutex;
    std::condition_variable cv;

    std::queue<std::pair<glm::ivec3, ChunkMeshData>> readyElements;
    std::mutex readyMutex;

    std::unique_ptr<IChunkMeshBuilder> meshBuilder;
};

#endif //CHUNKMESHINGTHREAD_H
