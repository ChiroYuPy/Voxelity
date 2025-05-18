//
// Created by adrian on 18/05/25.
//

#ifndef CHUNKMESHDATA_H
#define CHUNKMESHDATA_H

#include <vector>

class VoxelFace;

class ChunkMeshData {
public:
    void clear();
    void setVoxelFaces(std::vector<VoxelFace> faces);
    [[nodiscard]] const std::vector<VoxelFace>& getVoxelFaces() const;
    [[nodiscard]] bool hasVisibleFaces() const;

private:
    std::vector<VoxelFace> voxelFaces;
};

#endif //CHUNKMESHDATA_H
