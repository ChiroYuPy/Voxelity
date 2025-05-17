//
// Created by adrian on 13/05/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLT.h>

#include <memory>

#include "rendering/camera/CameraView.h"
#include "rendering/camera/CameraProjection.h"

class World;
class EventDispatcher;
class CameraController;
class ResizeListener;

class Application {
public:
    static Application& get();

    Application();
    ~Application();

    void update();

    void render();

    void run();

private:
    static Application* instance;
    GLFWwindow* window;
    CameraView cameraView;
    CameraProjection cameraProjection;

    std::unique_ptr<World> world;
    std::unique_ptr<EventDispatcher> eventDispatcher;
    std::unique_ptr<CameraController> cameraController;
    std::unique_ptr<ResizeListener> resizeListener;

    float lastTime;
};

#endif //APPLICATION_H
