cmake_minimum_required(VERSION 3.31)
project(Voxelity)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(externals/include)

add_executable(Voxelity
        main.cpp
        externals/src/glad.c
        src/Application.cpp
        src/rendering/Renderer.cpp
        src/rendering/Camera.cpp
        src/rendering/Shader.cpp
        src/blocks/BlockRegistry.cpp
        src/voxelWorld/Voxel.cpp
        include/voxelWorld/Chunk.h
        src/voxelWorld/Chunk.cpp
        include/voxelWorld/World.h
        src/voxelWorld/World.cpp
        include/rendering/VoxelFace.h
        src/listeners/CameraController.cpp
        externals/include/GLT.h
        include/events/EventType.h
        include/events/Event.h
        include/events/EventDispatcher.h
        include/events/EventListener.h
        include/events/GLFWEventAdapter.h
        src/events/GLFWEventAdapter.cpp
)

target_link_libraries(Voxelity glfw)
