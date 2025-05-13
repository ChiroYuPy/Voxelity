//
// Created by adrian on 13/05/25.
//

#include "../../include/rendering/Renderer.h"

Renderer::Renderer(const std::vector<FaceInstance>& instances_, const Shader& shader_)
: instances(instances_), shader(shader_) {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(FaceInstance), instances.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 3, GL_INT, sizeof(FaceInstance), static_cast<void *>(nullptr));
    glVertexAttribDivisor(0, 1);

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(FaceInstance), reinterpret_cast<void *>(offsetof(FaceInstance, faceId)));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FaceInstance), reinterpret_cast<void *>(offsetof(FaceInstance, color)));
    glVertexAttribDivisor(2, 1);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Renderer::render(const glm::mat4& view, const glm::mat4& projection) const {
    shader.use();
    shader.setUniform("uView", view);
    shader.setUniform("uProjection", projection);

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instances.size());
}