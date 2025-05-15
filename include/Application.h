//
// Created by adrian on 13/05/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLT.h>

#include <memory>

#include "rendering/Camera.h"

class World;
class CameraController;
class EventDispatcher;

class Application {
public:
    static Application& get();

    Application();
    ~Application();

    void update();

    void render() const;

    void run();

private:
    static Application* instance;
    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<World> world;
    std::unique_ptr<EventDispatcher> eventDispatcher;
    std::unique_ptr<CameraController> cameraController;

    const glm::mat4 projection;

    float lastTime;
};

#endif //APPLICATION_H
