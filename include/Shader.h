//
// Created by adrian on 13/05/25.
//

#ifndef SHADER_H
#define SHADER_H
#include <fstream>

#include "glad/glad.h"
#include <glm/glm.hpp>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    ~Shader();

    void use() const;

    void setUniform(const std::string& name, const glm::mat4& value) const;

    void setUniform(const std::string& name, const glm::vec3& value) const;

    void setUniform(const std::string& name, int value) const;

private:
    static std::string readFile(const char* path);

    static GLuint compileShader(GLenum type, const char* src);

    static void checkCompilationErrors(GLuint shader);

    static void checkLinkingErrors(GLuint program);
};

#endif //SHADER_H
