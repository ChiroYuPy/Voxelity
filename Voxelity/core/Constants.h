//
// Created by adrian on 16/05/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "voxelWorld/chunk/Voxel.h"

class Constants {
public:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr char* WINDOW_TITLE = "Voxelity";
    static constexpr float LOG_INTERVAL = 1.f;

    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
    static constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    static constexpr int RENDER_DISTANCE = 24;
    static constexpr int WORLD_HEIGHT = 8;

    static constexpr int RENDER_DISTANCE_VOXEL = RENDER_DISTANCE * CHUNK_SIZE;
    static constexpr int WORLD_HEIGHT_VOXEL = WORLD_HEIGHT * CHUNK_SIZE;

    static constexpr int BYTES_PER_VOXEL = sizeof(Voxel);
    static constexpr int BYTES_PER_CHUNK = CHUNK_VOLUME * BYTES_PER_VOXEL;

    static constexpr float NearPlane = 0.1f;
    static constexpr float FarPlane = 2000.f;
    static constexpr float FOV = 70.f;
    static constexpr float MinPitch = -89.f;
    static constexpr float MaxPitch = 89.f;
    static constexpr float PlayerReach = 8.f;

    static constexpr float MOUSE_SENSITIVITY = 0.2f;

    static constexpr float PLAYER_SPEED = 16.f;
    static constexpr float PLAYER_SPRINT_SPEED = 128.f;
};

#endif //CONSTANTS_H
