//
// Created by adrian on 17/05/25.
//

#include "CameraProjection.h"

CameraProjection::CameraProjection(const float fovDegrees, const float nearPlane, const float farPlane)
: fovDegrees(fovDegrees), nearPlane(nearPlane), farPlane(farPlane), aspectRatio(1.f),
projectionMatrix(), projectionMatrixDirty(true) {}

void CameraProjection::markProjectionMatrixDirty() {
    projectionMatrixDirty = true;
}

glm::mat4 CameraProjection::getProjectionMatrix() {
    if (projectionMatrixDirty) {
        projectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);
        projectionMatrixDirty = false;
    }
    return projectionMatrix;
}

void CameraProjection::setAspectRatio(const float aspectRatio_) {
    aspectRatio = aspectRatio_;
    markProjectionMatrixDirty();
}

void CameraProjection::setAspectRatio(const float width, const float height) {
    aspectRatio = width / height;
    markProjectionMatrixDirty();
}

void CameraProjection::setAspectRatio(const int width, const int height) {
    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    markProjectionMatrixDirty();
}

void CameraProjection::setFov(const float fovDegrees_) {
    fovDegrees = fovDegrees_;
    markProjectionMatrixDirty();
}