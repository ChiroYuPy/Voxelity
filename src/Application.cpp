//
// Created by adrian on 13/05/25.
//

#include "Application.h"

#include "listeners/CameraController.h"
#include "events/EventDispatcher.h"
#include "events/GLFWEventAdapter.h"
#include "voxelWorld/World.h"

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

Application* Application::instance = nullptr;

Application& Application::get() {
    return *instance;
}

Application::Application() : lastTime(0) {
    instance = this;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glEnable(GL_DEPTH_TEST);

    world = std::make_unique<World>();

    world->generateFromPosition(glm::ivec3(0, 0, 0));

    eventDispatcher = std::make_unique<EventDispatcher>();
    cameraController = std::make_unique<CameraController>(window, camera);
    eventDispatcher->subscribe(cameraController.get());
    GLFWEventAdapter(window, *eventDispatcher);
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 1000.f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        cameraController->update(deltaTime);
        glm::mat4 view = camera.getViewMatrix();

        world->generateFromPosition(camera.position);

        world->update();
        world->render(view, projection, glm::ivec3(1.0, 1.0, 1.0));

        glfwSwapBuffers(window);
    }
}