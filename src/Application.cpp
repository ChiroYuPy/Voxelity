//
// Created by adrian on 13/05/25.
//

#include "Application.h"
#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

Application::Application() : lastTime(0) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Voxel Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, InputManager::mouse_callback);

    glEnable(GL_DEPTH_TEST);

    const std::vector<FaceInstance> instances = {
        {{0, 0, 0}, 0, {1.0, 0.0, 0.0}},
        {{0, 0, 0}, 1, {0.8, 0.0, 0.0}},
        {{0, 0, 0}, 2, {0.0, 1.0, 0.0}},
        {{2, 0, 0}, 3, {0.0, 0.8, 0.0}},
        {{0, 0, 0}, 4, {0.0, 0.0, 1.0}},
        {{0, 0, 0}, 5, {0.0, 0.0, 0.8}},
    };

    shader = std::make_unique<Shader>("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");
    renderer = std::make_unique<Renderer>(instances, *shader);
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);
    glm::mat4 view = camera.getViewMatrix();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        InputManager::keyboard_callback(window, camera, deltaTime);
        view = camera.getViewMatrix();

        renderer->render(view, projection);

        glfwSwapBuffers(window);
    }
}