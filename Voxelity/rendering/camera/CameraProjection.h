//
// Created by adrian on 17/05/25.
//

#ifndef CAMERAPROJECTION_H
#define CAMERAPROJECTION_H

#include <GLT.h>

class CameraProjection {
public:
    float fovDegrees;
    float nearPlane;
    float farPlane;
    float aspectRatio;

    glm::mat4 projectionMatrix;
    bool projectionMatrixDirty;

    explicit CameraProjection(float fovDegrees, float nearPlane, float farPlane);

    void markProjectionMatrixDirty();

    [[nodiscard]] glm::mat4 getProjectionMatrix();

    void setAspectRatio(float aspectRatio_);

    void setAspectRatio(float width, float height);

    void setAspectRatio(int width, int height);

    void setFov(float fovDegrees_);
};

#endif //CAMERAPROJECTION_H
