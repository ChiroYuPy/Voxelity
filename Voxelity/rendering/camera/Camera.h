#ifndef CAMERA_H
#define CAMERA_H

#include <GLT.h>

class Camera {
public:
    glm::vec3 position;
    float yaw, pitch;

    glm::mat4 viewMatrix;
    bool viewMatrixDirty;

    explicit Camera(glm::vec3 startPos, float yaw, float pitch);

    void markViewMatrixDirty();

    [[nodiscard]] glm::mat4 getViewMatrix();

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
