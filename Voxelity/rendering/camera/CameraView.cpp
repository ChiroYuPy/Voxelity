//
// Created by adrian on 13/05/25.
//

#include "CameraView.h"

#include "core/Constants.h"

CameraView::CameraView(const glm::vec3 startPos, const float yaw, const float pitch)
: position(startPos), yaw(yaw), pitch(pitch),
  viewMatrix(), viewMatrixDirty(true),
  front(), right(), directionDirty(true) {}

void CameraView::rotate(const float dx, const float dy) {
    yaw += dx;
    pitch -= dy;
    pitch = glm::clamp(pitch, Constants::MinPitch, Constants::MaxPitch);
    markViewMatrixDirty();
    directionDirty = true;
}

glm::vec3 CameraView::getFront() const {
    if (directionDirty) updateDirectionVectors();
    return front;
}

glm::vec3 CameraView::getRight() const {
    if (directionDirty) updateDirectionVectors();
    return right;
}

void CameraView::updateDirectionVectors() const {
    front = glm::normalize(glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    });

    right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    directionDirty = false;
}

void CameraView::markViewMatrixDirty() const {
    viewMatrixDirty = true;
}

glm::mat4 CameraView::getViewMatrix() const {
    if (viewMatrixDirty) {
        viewMatrix = glm::lookAt(position, position + getFront(), glm::vec3(0, 1, 0));
        viewMatrixDirty = false;
    }
    return viewMatrix;
}

glm::vec3 CameraView::getPosition() const {
    return position;
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