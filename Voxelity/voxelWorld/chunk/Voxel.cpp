//
// Created by adrian on 13/05/25.
//

#include "voxelWorld/Voxel.h"

Voxel::Voxel() : data(0) {}

Voxel::Voxel(const BlockType type_) : data(0) {
    setType(type_);
}

Voxel::Voxel(const BlockType type_, const uint8_t variant, const uint8_t light) : data(0) {
    setType(type_);
    setVariant(variant);
    setLight(light);
}

[[nodiscard]] BlockType Voxel::getType() const {
    return static_cast<BlockType>(data & TYPE_MASK);
}

[[nodiscard]] uint8_t Voxel::getVariant() const {
    return data >> VARIANT_SHIFT & VARIANT_MASK;
}

[[nodiscard]] uint8_t Voxel::getLight() const {
    return data >> LIGHT_SHIFT & LIGHT_MASK;
}

void Voxel::setType(const BlockType type_) {
    data = data & ~TYPE_MASK | static_cast<uint32_t>(type_) & TYPE_MASK;
}

void Voxel::setType(const uint32_t type_) {
    data = data & ~TYPE_MASK | type_ & TYPE_MASK;
}

void Voxel::setVariant(const uint8_t variant) {
    data = data & ~(VARIANT_MASK << VARIANT_SHIFT) | (variant & VARIANT_MASK) << VARIANT_SHIFT;
}

void Voxel::setLight(const uint8_t light) {
    data = data & ~(LIGHT_MASK << LIGHT_SHIFT) | (light & LIGHT_MASK) << LIGHT_SHIFT;
}

[[nodiscard]] bool Voxel::isSolid() const {
    return BlockRegistry::isSolid(getType());
}

[[nodiscard]] bool Voxel::isVoid() const {
    return BlockRegistry::isVoid(getType());
}