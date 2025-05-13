cmake_minimum_required(VERSION 3.31)
project(Voxelity)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(externals/include)

add_executable(Voxelity
        main.cpp
        src/glad.c
        src/Application.cpp
        src/Renderer.cpp
        src/Camera.cpp
        src/Shader.cpp
)

target_link_libraries(Voxelity glfw)
