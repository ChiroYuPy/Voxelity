//
// Created by adrian on 13/05/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLT.h>

#include <memory>

#include "rendering/Camera.h"

class CameraController;
class EventDispatcher;
class Renderer;
class Shader;
class World;

class Application {
public:
    static Application& get();

    Application();
    ~Application();

    void run();

private:
    static Application* instance;
    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<World> world;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<EventDispatcher> eventDispatcher;
    std::unique_ptr<CameraController> cameraController;

    glm::vec3 lightDirection;

    float lastTime;
};

#endif //APPLICATION_H
