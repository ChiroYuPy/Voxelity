//
// Created by adrian on 14/05/25.
//

#ifndef NATURALWORLDGENERATOR_H
#define NATURALWORLDGENERATOR_H

#include "IWorldGenerator.h"

/**
 * @brief Concrete implementation of IChunkGenerator that generates natural terrain.
 *
 * This class provides voxel generation logic for natural world features such as
 * ground, hills, and other terrain types by overriding the generateVoxel method.
 */
class NaturaldGenerator final : public IChunkGenerator {
public:

    /**
    * @brief Default constructor for NaturalWorldGenerator.
    */
    NaturaldGenerator();

    /**
    * @brief Generates the voxel type at the given world position.
    *
    * This method defines the procedural rules for terrain generation,
    * returning the appropriate BlockType for the voxel at 'position'.
    *
    * @param position The 3D integer position in world coordinates to generate a voxel for.
    * @return The BlockType corresponding to the voxel at the specified position.
    */
    BlockType generateVoxel(const glm::ivec3& position) override;
};

#endif //NATURALWORLDGENERATOR_H
