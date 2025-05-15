//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/NaturalWorldGenerator.h"
#include "OS2.hpp"
#include <algorithm>

OS2 noise;

int getHeightAt(const int x, const int z) {
    constexpr double size = 1.0 / 256.0;
    // Bruit de base entre [-1, 1]
    auto noiseValue = static_cast<float>(noise.noise2(x * size, z * size));

    // Ramener entre [0, 1]
    noiseValue = (noiseValue + 1.0f) * 0.5f;

    // Déformation exponentielle pour accentuer les reliefs élevés
    // Exemples possibles :
    // - bruit^e (e.g. 1.5, 2) : creuse les vallées, accentue les sommets
    // - sqrt(bruit) : aplatie les hauts-reliefs
    constexpr float exponent = 2.2f;
    const float shaped = std::pow(noiseValue, exponent);  // accentue les pics

    const int height = static_cast<int>(shaped * 15); // max height
    return std::clamp(height, 0, 15);
}

NaturalWorldGenerator::NaturalWorldGenerator() = default;

BlockType NaturalWorldGenerator::generateVoxel(const glm::ivec3& position) {
    const int height = getHeightAt(position.x, position.z);
    if (position.y == 0) return BlockType::Bedrock;
    if (position.y == height + 1) return BlockType::Grass;
    if (0 < position.y && position.y < height + 1) return BlockType::Dirt;
    return BlockType::Air;
}
