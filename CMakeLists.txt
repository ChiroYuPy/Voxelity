cmake_minimum_required(VERSION 3.31)
project(Voxelity)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(externals/include)

add_executable(Voxelity
        main.cpp

        # librairies
        externals/src/glad.c
        externals/src/OS2.cpp

        # project
        src/Application.cpp
        src/rendering/Camera.cpp
        src/rendering/Shader.cpp
        src/blocks/BlockRegistry.cpp
        src/voxelWorld/Voxel.cpp
        src/voxelWorld/Chunk.cpp
        src/voxelWorld/World.cpp
        src/listeners/CameraController.cpp
        src/events/GLFWEventAdapter.cpp
        src/events/EventDispatcher.cpp
        src/voxelWorld/generators/FlatWorldGenerator.cpp
        src/voxelWorld/generators/NaturalWorldGenerator.cpp
        src/voxelWorld/generators/IWorldGenerator.cpp
        include/rendering/Mesh.h
        src/rendering/Mesh.cpp
)

target_link_libraries(Voxelity glfw)
