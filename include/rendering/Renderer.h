//
// Created by adrian on 13/05/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

class VoxelFace;

class Renderer {
public:
    GLuint vao, vbo;
    std::vector<VoxelFace> instances;
    Shader shader;

    explicit Renderer(const std::vector<VoxelFace>& instances_, const Shader& shader_);

    ~Renderer();

    void render(const glm::mat4& view, const glm::mat4& projection) const;
};

#endif //RENDERER_H
