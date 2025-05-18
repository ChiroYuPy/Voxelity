//
// Created by adrian on 18/05/25.
//

#include "ChunkMeshData.h"

#include "voxelWorld/chunk/VoxelFace.h"

void ChunkMeshData::clear() {
    voxelFaces.clear();
}

void ChunkMeshData::setVoxelFaces(std::vector<VoxelFace> faces) {
    voxelFaces = std::move(faces);
}

const std::vector<VoxelFace>& ChunkMeshData::getVoxelFaces() const {
    return voxelFaces;
}

bool ChunkMeshData::hasVisibleFaces() const {
    return !voxelFaces.empty();
}