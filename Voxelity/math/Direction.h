//
// Created by adrian on 15/05/25.
//

#ifndef DIRECTION_H
#define DIRECTION_H

#pragma once

#include <GLT.h>

enum Direction : uint8_t { XP, XM, YP, YM, ZP, ZM }; // directions

constexpr Direction DIRECTIONS[6] = { XP, XM, YP, YM, ZP, ZM };
constexpr Direction OPPOSITE_DIRECTIONS[6] = { XM, XP, YM, YP, ZM, ZP };

constexpr glm::ivec3 DIRECTION_NORMALS[6] = { // normals IT
    glm::ivec3( 1,  0,  0),  // XP
    glm::ivec3(-1,  0,  0),  // XM
    glm::ivec3( 0,  1,  0),  // YP
    glm::ivec3( 0, -1,  0),  // YM
    glm::ivec3( 0,  0,  1),  // ZP
    glm::ivec3( 0,  0, -1)   // ZM
};

inline glm::ivec3 getNormal(const Direction direction) {
    return DIRECTION_NORMALS[static_cast<uint8_t>(direction)];
}

inline Direction getOpposite(const Direction direction) {
    return OPPOSITE_DIRECTIONS[static_cast<uint8_t>(direction)];
}

#endif //DIRECTION_H
