//
// Created by adrian on 14/05/25.
//

#include "voxelWorld/generators/FlatGenerator.h"

BlockType FlatGenerator::generateVoxel(const glm::ivec3& position) {
   if (position.y == 4) return BlockType::Grass;
   if (position.y == 0) return BlockType::Bedrock;
   if (0 < position.y && position.y < 4) return BlockType::Dirt;
   return BlockType::Air;
}
