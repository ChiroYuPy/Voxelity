cmake_minimum_required(VERSION 3.31)
project(Voxelity)

set(CMAKE_CXX_STANDARD 20)

include_directories(Voxelity)
include_directories(externals/include)

add_executable(Voxelity
        Voxelity/main.cpp

        # librairies
        externals/src/glad.c    # glad
        externals/src/OS2.cpp   # open simplex 2
        externals/src/IMG.cpp   # stb image

        # project
        Voxelity/core/Application.cpp
        Voxelity/rendering/camera/CameraView.cpp
        Voxelity/rendering/shader/Shader.cpp
        Voxelity/blocks/BlockRegistry.cpp
        Voxelity/voxelWorld/chunk/Voxel.cpp
        Voxelity/voxelWorld/chunk/Chunk.cpp
        Voxelity/voxelWorld/World.cpp
        Voxelity/listeners/CameraController.cpp
        Voxelity/events/GLFWEventAdapter.cpp
        Voxelity/events/EventDispatcher.cpp
        Voxelity/voxelWorld/generators/FlatWorldGenerator.cpp
        Voxelity/voxelWorld/generators/NaturalWorldGenerator.cpp
        Voxelity/voxelWorld/generators/IWorldGenerator.cpp
        Voxelity/rendering/ChunkMesh.cpp
        Voxelity/textures/Image.cpp
        Voxelity/textures/Texture.cpp
        Voxelity/core/utils/Profiler.cpp
        Voxelity/voxelWorld/chunk/ChunkData.cpp
        Voxelity/voxelWorld/chunk/ChunkManager.cpp
        Voxelity/voxelWorld/chunk/ChunkRenderer.cpp
        Voxelity/voxelWorld/chunk/ChunkLoader.cpp
        Voxelity/listeners/ResizeListener.cpp
        Voxelity/listeners/ResizeListener.h
        Voxelity/rendering/camera/CameraProjection.cpp
        Voxelity/rendering/camera/CameraProjection.h
        Voxelity/threading/ConcurrentQueue.cpp
        Voxelity/threading/ConcurrentQueue.h
        Voxelity/threading/generation/GenerationTask.cpp
        Voxelity/threading/generation/GenerationTask.h
        Voxelity/threading/ThreadPool.cpp
        Voxelity/threading/ThreadPool.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer")

target_link_libraries(Voxelity glfw)
