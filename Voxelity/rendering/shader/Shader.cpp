//
// Created by adrian on 13/05/25.
//

#include "Shader.h"

#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    const std::string vertexCode = readFile(vertexPath);
    const std::string fragmentCode = readFile(fragmentPath);

    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkLinkingErrors(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string& name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(const GLenum type, const char* src) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    checkCompilationErrors(shader);
    return shader;
}

void Shader::checkCompilationErrors(const GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error:\n" << infoLog << std::endl;
    }
}

void Shader::checkLinkingErrors(const GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program Linking Error:\n" << infoLog << std::endl;
    }
}