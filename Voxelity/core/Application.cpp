//
// Created by adrian on 13/05/25.
//

#include "Application.h"

#include <iostream>

#include "utils/Profiler.h"
#include "listeners/CameraController.h"
#include "events/EventDispatcher.h"
#include "events/GLFWEventAdapter.h"
#include "listeners/ResizeListener.h"
#include "voxelWorld/World.h"
#include "voxelWorld/generators/NaturalWorldGenerator.h"

constexpr unsigned int SCREEN_WIDTH = 2560;
constexpr unsigned int SCREEN_HEIGHT = 1600;

Application* Application::instance = nullptr;

Application& Application::get() {
    return *instance;
}

Application::Application()
: cameraView({0, 0, 0}, 0, 0), cameraProjection(Constants::FOV, Constants::NearPlane, Constants::FarPlane, Constants::FOV), lastTime(0) {
    instance = this;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    world = std::make_unique<World>(std::make_unique<NaturalWorldGenerator>());

    eventDispatcher = std::make_unique<EventDispatcher>();

    cameraController = std::make_unique<CameraController>(window, cameraView);
    eventDispatcher->subscribe(cameraController.get());

    resizeListener = std::make_unique<ResizeListener>(window, cameraProjection);
    eventDispatcher->subscribe(resizeListener.get());

    GLFWEventAdapter(window, *eventDispatcher);

    world->updateFromPlayerPosition(cameraView.position);
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
    world->updateFromPlayerPosition(cameraView.position);
    world->update();
}

void Application::render() {
    PROFILE_FUNCTION();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = cameraView.getViewMatrix();
    const glm::mat4 projection = cameraProjection.getProjectionMatrix();

    world->render(cameraView.position, view, projection,
                 glm::vec3(0.5f, -1.0f, 0.3f),
                 glm::vec3(1.0f, 0.95f, 0.8f),
                 glm::vec3(0.25f, 0.25f, 0.3f));


    glfwSwapBuffers(window);
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        PROFILE_SCOPE("MainLoop");
        glfwPollEvents();

        update();
        render();
    }
}