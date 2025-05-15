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
        Voxelity/Application.cpp
        Voxelity/rendering/camera/Camera.cpp
        Voxelity/rendering/Shader.cpp
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
        Voxelity/Profiler.cpp
        Voxelity/rendering/shader/IShader.h
        Voxelity/rendering/shader/OpenGLShader.cpp
        Voxelity/rendering/shader/OpenGLShader.h
        Voxelity/rendering/shader/ShaderLoader.cpp
        Voxelity/rendering/shader/ShaderLoader.h
        Voxelity/rendering/mesh/IMesh.h
        Voxelity/rendering/mesh/OpenGLMesh.cpp
        Voxelity/rendering/mesh/OpenGLMesh.h
        Voxelity/rendering/texture/ITexture.h
        Voxelity/rendering/material/Material.cpp
        Voxelity/rendering/material/Material.h
        Voxelity/rendering/texture/OpenGLTexture2D.cpp
        Voxelity/rendering/texture/OpenGLTexture2D.h
        Voxelity/resources/ResourceCache.cpp
        Voxelity/resources/ResourceCache.h
        Voxelity/resources/ResourceManager.cpp
        Voxelity/resources/ResourceManager.h
        Voxelity/resources/TextureLoader.h
)

target_link_libraries(Voxelity glfw)
