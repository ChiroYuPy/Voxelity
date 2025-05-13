//
// Created by adrian on 13/05/25.
//

#include "rendering/Camera.h"

Camera::Camera(const glm::vec3 startPos, const float yaw, const float pitch) : position(startPos), yaw(yaw), pitch(pitch) {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getFront(), glm::vec3(0, 1, 0));
}

// Mouvement dans la direction actuelle de la caméra
void Camera::moveForward(const float deltaTime) {
    position += getFront() * speed * deltaTime;
}

void Camera::moveBackward(const float deltaTime) {
    position -= getFront() * speed * deltaTime;
}

void Camera::moveRight(const float deltaTime) {
    position += getRight() * speed * deltaTime;
}

void Camera::moveLeft(const float deltaTime) {
    position -= getRight() * speed * deltaTime;
}

void Camera::moveUp(const float deltaTime) {
    position += glm::vec3(0, 1, 0) * speed * deltaTime;
}

void Camera::moveDown(const float deltaTime) {
    position -= glm::vec3(0, 1, 0) * speed * deltaTime;
}

void Camera::rotate(const float dx, const float dy) {
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;
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