//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/NaturalWorldGenerator.h"
#include "OS2.hpp"
#include <algorithm>

OS2 noise;

int getHeightAt(const int x, const int z) {
    constexpr double size = 1.f / 64.f;
    const auto noiseValue = static_cast<float>(noise.noise2(static_cast<float>(x * size), static_cast<float>(z * size)));
    const int height = static_cast<int>((noiseValue + 1.0f) * 64);
    return std::clamp(height, 0, 128);
}

NaturalWorldGenerator::NaturalWorldGenerator() = default;

BlockType NaturalWorldGenerator::generateVoxel(const glm::ivec3& position) {
    const int height = getHeightAt(position.x, position.z);
    if (position.y == 0) return BlockType::Bedrock;
    if (position.y == height) return BlockType::Grass;
    if (0 < position.y && position.y < height) return BlockType::Dirt;
    return BlockType::Air;
}

