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
        Voxelity/rendering/camera/Camera.cpp
        Voxelity/rendering/shader/Shader.cpp
        Voxelity/blocks/BlockRegistry.cpp
        Voxelity/voxelWorld/Voxel.cpp
        Voxelity/voxelWorld/Chunk.cpp
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
        Voxelity/voxelWorld/ChunkData.cpp
        Voxelity/voxelWorld/ChunkManager.cpp
        Voxelity/voxelWorld/ChunkRenderer.cpp
        Voxelity/voxelWorld/ChunkLoader.cpp
        Voxelity/listeners/ResizeListener.cpp
        Voxelity/listeners/ResizeListener.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer")

target_link_libraries(Voxelity glfw)
