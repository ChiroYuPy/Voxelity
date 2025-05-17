//
// Created by adrian on 18/05/25.
//

#ifndef GENERATIONTASK_H
#define GENERATIONTASK_H

#include "core/Constants.h"
#include "voxelWorld/chunk/Chunk.h"

struct GenerationTask {
    Chunk* chunk;

    explicit GenerationTask(Chunk* c) : chunk(c) {}

    void execute() const {
        chunk->getData().clear();

        for (int i = 0; i < Constants::ChunkVolume; ++i) {
            chunk->getData().fill(BlockType::Bedrock);
        }

        chunk->setState(ChunkState::Generated); // generation task ended
    }
};

#endif //GENERATIONTASK_H
