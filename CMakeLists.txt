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
        src/Voxel.cpp
)

target_link_libraries(Voxelity glfw)
