//
// Created by adrian on 13/05/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>

#include "Camera.h"
#include "Renderer.h"

class GLFWwindow;

class Application {
public:
    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Renderer> renderer;
    float lastTime;

    Application();

    ~Application();

    void run();
};

#endif //APPLICATION_H
