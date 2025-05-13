//
// Created by adrian on 13/05/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

class FaceInstance {
public:
    glm::ivec3 pos;
    int faceId;
    glm::vec3 color;

    FaceInstance(const glm::ivec3 p, const int id, const glm::vec3 c) : pos(p), faceId(id), color(c) {}
};

class Renderer {
public:
    GLuint vao{}, vbo{};
    std::vector<FaceInstance> instances;
    Shader shader;

    explicit Renderer(const std::vector<FaceInstance>& instances_, const Shader& shader_);

    ~Renderer();

    void render(const glm::mat4& view, const glm::mat4& projection) const;
};

#endif //RENDERER_H
