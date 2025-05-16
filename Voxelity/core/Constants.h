//
// Created by adrian on 16/05/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants {
public:
    static constexpr int ChunkSize = 16;
    static constexpr int ChunkArea = ChunkSize * ChunkSize;
    static constexpr int ChunkVolume = ChunkSize * ChunkSize * ChunkSize;

    static constexpr int RenderDistance = 16;
    static constexpr int RenderHeight = 2;
    static constexpr int WorldHeight = RenderHeight * ChunkSize;

    static constexpr int VOXELS_PER_CHUNK = ChunkVolume;
    static constexpr int BYTES_PER_VOXEL = sizeof(uint8_t);
    static constexpr int BYTES_PER_CHUNK = VOXELS_PER_CHUNK * BYTES_PER_VOXEL;
};

#endif //CONSTANTS_H
