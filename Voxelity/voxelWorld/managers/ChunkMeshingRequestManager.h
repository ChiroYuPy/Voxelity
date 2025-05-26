//
// Created by adrian on 20/05/25.
//

#ifndef CHUNKMESHINGREQUESTMANAGER_H
#define CHUNKMESHINGREQUESTMANAGER_H

#include <memory>
#include "threads/meshing/ChunkMeshingThread.h"

class ChunkStorage;

class ChunkMeshingRequestManager {
public:
    explicit ChunkMeshingRequestManager(std::unique_ptr<IChunkMeshBuilder> meshBuilder);
    ~ChunkMeshingRequestManager();

    void enqueueDirtyChunks(ChunkStorage& worldChunkData) const;
    void processReadyMeshes(ChunkStorage& worldChunkData) const;

private:
    std::unique_ptr<ChunkMeshingThread> chunkMeshingThread;
};

#endif //CHUNKMESHINGREQUESTMANAGER_H
