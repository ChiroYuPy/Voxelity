cmake_minimum_required(VERSION 3.31)
project(Voxelity)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(externals/include)

add_executable(Voxelity
        main.cpp

        # librairies
        externals/src/glad.c    # glad
        externals/src/OS2.cpp   # open simplex 2
        externals/src/IMG.cpp   # stb image

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
        src/rendering/ChunkMesh.cpp
        src/textures/Image.cpp
        src/textures/Texture.cpp
        src/Profiler.cpp
)

target_link_libraries(Voxelity glfw)
