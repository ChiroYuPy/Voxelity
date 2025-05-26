//
// Created by adrian on 13/05/25.
//

#include "Application.h"

#include <iostream>

#include "utils/Profiler.h"
#include "listeners/CameraController.h"
#include "events/EventDispatcher.h"
#include "events/GLFWEventAdapter.h"
#include "listeners/PlayerController.h"
#include "listeners/ResizeListener.h"
#include "utils/Logger.h"
#include "voxelWorld/World.h"
#include "voxelWorld/generators/NaturaldGenerator.h"
#include "voxelWorld/meshBuilders/FaceCullingMeshBuilder.h"

Application* Application::instance = nullptr;

Application& Application::get() {
    return *instance;
}

Application::Application()
: cameraView({0, Constants::WINDOW_HEIGHT, 0}, 0, 0), cameraProjection(Constants::FOV, Constants::NearPlane, Constants::FarPlane), lastTime(0), running(false) {
    Logger::info("Initialisation de l'application");

    cameraProjection.setAspectRatio(static_cast<float>(Constants::WINDOW_WIDTH) / static_cast<float>(Constants::WINDOW_HEIGHT));
    Logger::debug("Aspect ratio fixé");

    instance = this;

    if (!glfwInit()) {
        Logger::fatal("Impossible d'initialiser GLFW");
    }
    Logger::info("GLFW initialisé");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, "Voxel Renderer", nullptr, nullptr);
    if (!window) {
        Logger::fatal("Échec de la création de la fenêtre GLFW");
    }
    Logger::info("Fenêtre GLFW créée");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        Logger::fatal("Échec de l'initialisation de GLAD");
    }
    Logger::info("GLAD initialisé");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    Logger::info("OpenGL configuré (clear color, depth test, cull face)");

    world = std::make_unique<World>(std::make_unique<FaceCullingMeshBuilder>(), std::make_unique<NaturaldGenerator>());
    Logger::info("Monde voxel initialisé");

    eventDispatcher = std::make_unique<EventDispatcher>();
    Logger::info("Event dispatcher créé");

    cameraController = std::make_unique<CameraController>(window, cameraView);
    eventDispatcher->subscribe(cameraController.get());
    Logger::info("CameraController abonné aux événements");

    resizeListener = std::make_unique<ResizeListener>(window, cameraProjection);
    eventDispatcher->subscribe(resizeListener.get());
    Logger::info("ResizeListener abonné aux événements");

    playerController = std::make_unique<PlayerController>(cameraView, world->getWorldChunkData());
    eventDispatcher->subscribe(playerController.get());
    Logger::info("PlayerController abonné aux événements");

    GLFWEventAdapter(window, *eventDispatcher);
    Logger::info("Adaptateur d'événements GLFW configuré");

    world->updateFromPlayerPosition(cameraView.getPosition());
    Logger::info("Mise à jour initiale du monde depuis la position du joueur");
}

Application::~Application() {
    Logger::info("Destruction de l'application");

    world.reset();
    eventDispatcher.reset();
    cameraController.reset();
    resizeListener.reset();

    glfwDestroyWindow(window);
    glfwTerminate();

    Logger::info("Ressources GLFW libérées");
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
        Logger::info("FPS: " + std::to_string(frames));
        frames = 0;
        lastPrint = currentTime;
    }

    cameraController->update(deltaTime);

    world->updateFromPlayerPosition(cameraView.getPosition());

    world->update();
}

void Application::render() {
    PROFILE_FUNCTION();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = cameraView.getViewMatrix();
    const glm::mat4 projection = cameraProjection.getProjectionMatrix();

    world->render(cameraView.getPosition(), view, projection,
                 glm::vec3(0.5f, -1.0f, 0.3f),
                 glm::vec3(1.0f, 0.95f, 0.8f),
                 glm::vec3(0.25f, 0.25f, 0.3f));

    glfwSwapBuffers(window);
}

void Application::run() {
    Logger::separator("Game Loop");
    Logger::info("Démarrage de la boucle principale");
    running = true;
    while (running) {
        PROFILE_SCOPE("MainLoop");
        glfwPollEvents();
        update();
        render();

        running = !glfwWindowShouldClose(window);
    }
    Logger::info("Exited game loop.");
}