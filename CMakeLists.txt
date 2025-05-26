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
        Voxelity/voxelWorld/generators/FlatGenerator.cpp
        Voxelity/voxelWorld/generators/NaturaldGenerator.cpp
        Voxelity/voxelWorld/generators/IWorldGenerator.cpp
        Voxelity/voxelWorld/chunk/ChunkMesh.cpp
        Voxelity/textures/Image.cpp
        Voxelity/textures/Texture.cpp
        Voxelity/core/utils/Profiler.cpp
        Voxelity/voxelWorld/chunk/ChunkData.cpp
        Voxelity/voxelWorld/WorldChunkData.cpp
        Voxelity/voxelWorld/WorldChunkRenderer.cpp
        Voxelity/voxelWorld/managers/ChunkGenerationRequestManager.cpp
        Voxelity/listeners/ResizeListener.cpp
        Voxelity/rendering/camera/CameraProjection.cpp
        Voxelity/threads/generation/ChunkGenerationThread.cpp
        Voxelity/voxelWorld/meshBuilders/FaceCullingMeshBuilder.cpp
        Voxelity/voxelWorld/utils/Raycaster.cpp
        Voxelity/listeners/PlayerController.cpp
        Voxelity/voxelWorld/chunk/ChunkMeshData.cpp
        Voxelity/threads/meshing/ChunkMeshingThread.cpp
        Voxelity/voxelWorld/meshBuilders/ChunkDataNeighborhood.cpp
        Voxelity/voxelWorld/meshBuilders/ChunkDataNeighborhood.h
        Voxelity/voxelWorld/managers/ChunkMeshingRequestManager.cpp
        Voxelity/voxelWorld/managers/ChunkMeshingRequestManager.h
        Voxelity/rendering/graphics/Pointer.cpp
        Voxelity/rendering/graphics/Pointer.h
        Voxelity/threads/ITask.h
        Voxelity/core/utils/Logger.cpp
        Voxelity/core/utils/Logger.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer")

target_link_libraries(Voxelity glfw)
