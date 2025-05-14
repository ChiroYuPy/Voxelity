//
// Created by adrian on 13/05/25.
//

#include "Application.h"

#include "listeners/CameraController.h"
#include "events/EventDispatcher.h"
#include "events/GLFWEventAdapter.h"

#include "rendering/VoxelFace.h"

#include "rendering/Renderer.h"
#include "rendering/Shader.h"
#include "voxelWorld/World.h"

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

    window = glfwCreateWindow(800, 600, "Voxel Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glEnable(GL_DEPTH_TEST);

    const std::vector<VoxelFace> instances = {
        {{0, 0, 0}, 0, {1.0, 0.0, 0.0}},
        {{0, 0, 0}, 1, {0.8, 0.0, 0.0}},
        {{0, 0, 0}, 2, {0.0, 1.0, 0.0}},
        {{2, 0, 0}, 3, {0.0, 0.8, 0.0}},
        {{0, 0, 0}, 4, {0.0, 0.0, 1.0}},
        {{0, 0, 0}, 5, {0.0, 0.0, 0.8}},
    };

    world = std::make_unique<World>();
    auto& chunk = world->getOrCreateChunk(0, 0, 0);

    for (int z = 0; z < CHUNK_SIZE; ++z)
        for (int y = 0; y < CHUNK_SIZE; ++y)
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                if (y <= 2) chunk.set(x, y, z, Voxel{static_cast<uint8_t>((y + 1) % 3 + 1)});
            }

    std::vector<VoxelFace> faces = world->generateFaceInstances();
    shader = std::make_unique<Shader>("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");
    renderer = std::make_unique<Renderer>(faces, *shader);
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
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);
    glm::mat4 view = camera.getViewMatrix();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        cameraController->update(deltaTime);
        view = camera.getViewMatrix();

        renderer->render(view, projection);

        glfwSwapBuffers(window);
    }
}