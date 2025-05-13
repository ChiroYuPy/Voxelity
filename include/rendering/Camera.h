#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    float yaw, pitch;
    float speed = 5.0f;
    float sensitivity = 0.1f;

    explicit Camera(glm::vec3 startPos = glm::vec3(0, 0, 3),
                    float yaw = -90.0f,
                    float pitch = 0.0f);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    void moveForward(float deltaTime);

    void moveBackward(float deltaTime);

    void moveRight(float deltaTime);

    void moveLeft(float deltaTime);

    void moveUp(float deltaTime);

    void moveDown(float deltaTime);

    void rotate(float dx, float dy);

private:
    [[nodiscard]] glm::vec3 getFront() const;

    [[nodiscard]] glm::vec3 getRight() const;
};

#endif // CAMERA_H
