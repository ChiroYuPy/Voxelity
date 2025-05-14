//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/NaturalWorldGenerator.h"
#include "OS2.hpp"
#include <algorithm>

OS2 noise;

int getHeightAt(const int x, const int z) {
    const auto noiseValue = static_cast<float>(noise.noise2(static_cast<float>(x), static_cast<float>(z)));
    const int height = static_cast<int>((noiseValue + 1.0f) * 5);
    return std::clamp(height, 0, 10);
}

NaturalWorldGenerator::NaturalWorldGenerator() = default;

BlockType NaturalWorldGenerator::generateVoxel(const glm::ivec3& position) {
    if (const int height = getHeightAt(position.x, position.z); position.y == height) {
        return BlockType::Grass;
    } else {
        if (position.y < height) {
            if (position.y == height - 1) return BlockType::Dirt;
            if (position.y == 0) return BlockType::Bedrock;
            return BlockType::Stone;
        }
        return BlockType::Air;
    }
}

