//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/NaturaldGenerator.h"
#include "OS2.hpp"
#include <algorithm>

#include "core/Constants.h"
#include "voxelWorld/World.h"

OS2 noise;

int getHeightAt(const int x, const int z) {
    constexpr double size = 1.0 / 64.0;
    auto noiseValue = static_cast<float>(noise.noise2(x * size, z * size));

    noiseValue = (noiseValue + 1.0f) * 0.5f;

    constexpr float exponent = 2.f;
    const float shaped = std::pow(noiseValue, exponent);

    constexpr int worldHeight = Constants::WorldChunkHeight * Constants::ChunkSize - 1;
    static constexpr int startHeight = worldHeight / 2;
    const int height = startHeight + static_cast<int>(shaped * (worldHeight - startHeight)); // max height
    return std::clamp(height, startHeight, worldHeight);
}

NaturaldGenerator::NaturaldGenerator() = default;

BlockType NaturaldGenerator::generateVoxel(const glm::ivec3& position) {
    const int height = getHeightAt(position.x, position.z);

    if (position.y == 0)
        return BlockType::Bedrock;

    if (position.y < height - 4)
        return BlockType::Stone;

    if (position.y < height - 1)
        return BlockType::Dirt;

    if (position.y == height - 1)
        return BlockType::Grass;

    return BlockType::Air;
}
