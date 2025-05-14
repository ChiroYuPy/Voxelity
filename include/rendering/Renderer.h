//
// Created by adrian on 13/05/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <GLT.h>

#include <vector>

#include "Shader.h"

class VoxelFace;

class Renderer {
public:
    GLuint vao, vbo;
    std::vector<VoxelFace> voxelFaces;
    Shader shader;

    explicit Renderer(const std::vector<VoxelFace>& instances_, const Shader& shader_);

    ~Renderer();

    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDirection) const;
};

#endif //RENDERER_H
