#ifndef CAMERA_H
#define CAMERA_H

#include <GLT.h>

class Camera {
public:
    glm::vec3 position;
    float yaw, pitch;

    explicit Camera(glm::vec3 startPos = glm::vec3(0, 0, 0),
                    float yaw = -90.0f,
                    float pitch = 0.0f);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    void moveForward(float amount);

    void moveBackward(float amount);

    void moveRight(float amount);

    void moveLeft(float amount);

    void moveUp(float amount);

    void moveDown(float amount);

    void rotate(float dx, float dy);

private:
    [[nodiscard]] glm::vec3 getFront() const;

    [[nodiscard]] glm::vec3 getRight() const;
};

#endif // CAMERA_H
