//
// Created by adrian on 15/05/25.
//

#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <array>

struct Plane {
    glm::vec3 n;
    float d;

    [[nodiscard]] float signedDist(const glm::vec3& p) const {
        return glm::dot(n, p) + d;
    }
};

class Frustum {
public:
    std::array<Plane,6> planes{};

    void update(const glm::mat4& viewProj) {
        if (viewProj == cachedViewProj) return;
        cachedViewProj = viewProj;

        const auto m = reinterpret_cast<const float *>(&viewProj);

        // Left :  m[3] + m[0]
        planes[0].n.x = m[ 3] + m[ 0];
        planes[0].n.y = m[ 7] + m[ 4];
        planes[0].n.z = m[11] + m[ 8];
        planes[0].d   = m[15] + m[12];
        // Right : m[3] - m[0]
        planes[1].n.x = m[ 3] - m[ 0];
        planes[1].n.y = m[ 7] - m[ 4];
        planes[1].n.z = m[11] - m[ 8];
        planes[1].d   = m[15] - m[12];
        // Bottom : m[3] + m[1]
        planes[2].n.x = m[ 3] + m[ 1];
        planes[2].n.y = m[ 7] + m[ 5];
        planes[2].n.z = m[11] + m[ 9];
        planes[2].d   = m[15] + m[13];
        // Top : m[3] - m[1]
        planes[3].n.x = m[ 3] - m[ 1];
        planes[3].n.y = m[ 7] - m[ 5];
        planes[3].n.z = m[11] - m[ 9];
        planes[3].d   = m[15] - m[13];
        // Near : m[3] + m[2]
        planes[4].n.x = m[ 3] + m[ 2];
        planes[4].n.y = m[ 7] + m[ 6];
        planes[4].n.z = m[11] + m[10];
        planes[4].d   = m[15] + m[14];
        // Far : m[3] - m[2]
        planes[5].n.x = m[ 3] - m[ 2];
        planes[5].n.y = m[ 7] - m[ 6];
        planes[5].n.z = m[11] - m[10];
        planes[5].d   = m[15] - m[14];

        for (auto&[n, d] : planes) {
            const float len = glm::length(n);
            n   /= len;
            d   /= len;
        }
    }

    [[nodiscard]] bool intersectsAABB(const glm::vec3& boxCenter, const glm::vec3& boxHalfExtents) const {
        for (const auto& p : planes) {
            const float r = boxHalfExtents.x * std::abs(p.n.x)
                    + boxHalfExtents.y * std::abs(p.n.y)
                    + boxHalfExtents.z * std::abs(p.n.z);
            if (const float s = p.signedDist(boxCenter); s + r < 0) return false;
        }
        return true;
    }

private:
    glm::mat4 cachedViewProj = glm::mat4(0);
};

#endif //FRUSTRUM_H
