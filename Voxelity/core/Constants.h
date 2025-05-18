//
// Created by adrian on 16/05/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

class Constants {
public:
    static constexpr int ChunkSize = 16;
    static constexpr int ChunkArea = ChunkSize * ChunkSize;
    static constexpr int ChunkVolume = ChunkSize * ChunkSize * ChunkSize;

    static constexpr int RenderDistance = 16;
    static constexpr int WorldChunkHeight = 16;
    static constexpr int WorldHeight = WorldChunkHeight * ChunkSize;

    static constexpr int VOXELS_PER_CHUNK = ChunkVolume;
    static constexpr int BYTES_PER_VOXEL = sizeof(std::uint8_t);
    static constexpr int BYTES_PER_CHUNK = VOXELS_PER_CHUNK * BYTES_PER_VOXEL;

    static constexpr float NearPlane = 0.1f;
    static constexpr float FarPlane = 2000.f;
    static constexpr float FOV = 70.f;
    static constexpr float MinPitch = -89.f;
    static constexpr float MaxPitch = 89.f;
    static constexpr float PlayerReach = 8.f;
};

#endif //CONSTANTS_H
