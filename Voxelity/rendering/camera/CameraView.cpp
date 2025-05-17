//
// Created by adrian on 13/05/25.
//

#include "CameraView.h"

#include "core/Constants.h"

CameraView::CameraView(const glm::vec3 startPos, const float yaw, const float pitch)
: position(startPos), yaw(yaw), pitch(pitch), viewMatrix(), viewMatrixDirty(true) {}

void CameraView::markViewMatrixDirty() {
    viewMatrixDirty = true;
}

glm::mat4 CameraView::getViewMatrix() {
    if (viewMatrixDirty) {
        viewMatrix = glm::lookAt(position, position + getFront(), glm::vec3(0, 1, 0));
        viewMatrixDirty = false;
    }
    return viewMatrix;
}

void CameraView::moveForward(const float amount) {
    glm::vec3 flatFront = getFront();
    flatFront.y = 0;
    position += glm::normalize(flatFront) * amount;
    markViewMatrixDirty();
}

void CameraView::moveBackward(const float amount) {
    glm::vec3 flatFront = getFront();
    flatFront.y = 0;
    position -= glm::normalize(flatFront) * amount;
    markViewMatrixDirty();
}

void CameraView::moveRight(const float amount) {
    position += getRight() * amount;
    markViewMatrixDirty();
}

void CameraView::moveLeft(const float amount) {
    position -= getRight() * amount;
    markViewMatrixDirty();
}

void CameraView::moveUp(const float amount) {
    position += glm::vec3(0, 1, 0) * amount;
    markViewMatrixDirty();
}

void CameraView::moveDown(const float amount) {
    position -= glm::vec3(0, 1, 0) * amount;
    markViewMatrixDirty();
}

void CameraView::rotate(const float dx, const float dy) {
    yaw += dx;
    pitch -= dy;
    pitch = glm::clamp(pitch, Constants::MinPitch, Constants::MaxPitch);
    markViewMatrixDirty();
}

glm::vec3 CameraView::getFront() const {
    return glm::normalize(glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    });
}

glm::vec3 CameraView::getRight() const {
    return glm::normalize(glm::cross(getFront(), glm::vec3(0, 1, 0)));
}