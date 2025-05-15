//
// Created by adrian on 13/05/25.
//

#include "rendering/Camera.h"

Camera::Camera(const glm::vec3 startPos, const float yaw, const float pitch) : position(startPos), yaw(yaw), pitch(pitch) {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getFront(), glm::vec3(0, 1, 0));
}

void Camera::moveForward(const float amount) {
    glm::vec3 flatFront = getFront();
    flatFront.y = 0;
    position += glm::normalize(flatFront) * amount;
}

void Camera::moveBackward(const float amount) {
    glm::vec3 flatFront = getFront();
    flatFront.y = 0;
    position -= glm::normalize(flatFront) * amount;
}

void Camera::moveRight(const float amount) {
    position += getRight() * amount;
}

void Camera::moveLeft(const float amount) {
    position -= getRight() * amount;
}

void Camera::moveUp(const float amount) {
    position += glm::vec3(0, 1, 0) * amount;
}

void Camera::moveDown(const float amount) {
    position -= glm::vec3(0, 1, 0) * amount;
}

void Camera::rotate(const float dx, const float dy) {
    yaw += dx;
    pitch -= dy;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

glm::vec3 Camera::getFront() const {
    return glm::normalize(glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    });
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(getFront(), glm::vec3(0, 1, 0)));
}