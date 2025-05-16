//
// Created by adrian on 13/05/25.
//

#include "Application.h"

#include <iostream>

#include "../core/utils/Profiler.h"
#include "../listeners/CameraController.h"
#include "../events/EventDispatcher.h"
#include "../events/GLFWEventAdapter.h"
#include "../voxelWorld/World.h"

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 720;

Application* Application::instance = nullptr;

Application& Application::get() {
    return *instance;
}

Application::Application()
  : projection(glm::perspective(glm::radians(45.0f),
    static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
    0.1f, 1000.f)),
    lastTime(0)
{
    instance = this;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    world = std::make_unique<World>();

    eventDispatcher = std::make_unique<EventDispatcher>();
    cameraController = std::make_unique<CameraController>(window, camera);
    eventDispatcher->subscribe(cameraController.get());
    GLFWEventAdapter(window, *eventDispatcher);

    world->generateFromPosition(camera.position);
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::update() {
    PROFILE_FUNCTION();
    const auto currentTime = static_cast<float>(glfwGetTime());
    const float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    static int frames = 0;
    static float lastPrint = 0.0f;

    frames++;
    if (currentTime - lastPrint >= 1.0f) {
        std::cout << "FPS: " << frames << std::endl;
        frames = 0;
        lastPrint = currentTime;
    }

    cameraController->update(deltaTime);
    // world->generateFromPosition(camera.position);
    world->update();
}

void Application::render() const {
    PROFILE_FUNCTION();

    {
        PROFILE_SCOPE("glClear");
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    const glm::mat4 view = camera.getViewMatrix();

    {
        PROFILE_SCOPE("world.render");
        world->render(view, projection,
                     glm::vec3(0.5f, -1.0f, 0.3f),
                     glm::vec3(1.0f, 0.95f, 0.8f),
                     glm::vec3(0.25f, 0.25f, 0.3f));
    }

    {
        PROFILE_SCOPE("swapBuffers");
        glfwSwapBuffers(window);
    }
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        PROFILE_SCOPE("MainLoop");
        glfwPollEvents();

        update();
        render();
    }
}