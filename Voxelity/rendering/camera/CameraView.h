#ifndef CAMERA_H
#define CAMERA_H

#include <GLT.h>

class CameraView {
    glm::vec3 position;
    float yaw, pitch;

    mutable glm::mat4 viewMatrix;
    mutable bool viewMatrixDirty;

    mutable glm::vec3 front, right;
    mutable bool directionDirty;

    void updateDirectionVectors() const;

public:
    CameraView(glm::vec3 startPos, float yaw, float pitch);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getFront() const;
    glm::vec3 getRight() const;

    void moveForward(float amount);
    void moveBackward(float amount);
    void moveRight(float amount);
    void moveLeft(float amount);
    void moveUp(float amount);
    void moveDown(float amount);

    void rotate(float dx, float dy);
    void markViewMatrixDirty() const;

    glm::vec3 getPosition() const;
};

#endif // CAMERA_H
