//
// Created by adrian on 13/05/25.
//

#ifndef VOXEL_H
#define VOXEL_H

#include "blocks/BlockType.h"
#include "blocks/BlockRegistry.h"

struct Voxel {
private:
    uint32_t data;

    static constexpr uint32_t TYPE_BITS         = 10;                           // 1024 types
    static constexpr uint32_t METADATA_BITS     = 6;                            // 64 variants
    static constexpr uint32_t LIGHT_BITS        = 4;                            // 16 light states
    static constexpr uint32_t RESERVED_BITS     = 12;                           // 4096 values reserved for other usages

    static constexpr uint32_t TYPE_MASK         = (1u << TYPE_BITS) - 1;        // 0x3FF
    static constexpr uint32_t VARIANT_MASK      = (1u << METADATA_BITS) - 1;    // 0x3F
    static constexpr uint32_t LIGHT_MASK        = (1u << LIGHT_BITS) - 1;       // 0xF

    static constexpr uint32_t VARIANT_SHIFT     = TYPE_BITS;                    // 10
    static constexpr uint32_t LIGHT_SHIFT       = TYPE_BITS + METADATA_BITS;    // 16

public:
    explicit Voxel();

    explicit Voxel(BlockType type_);

    explicit Voxel(BlockType type_, uint8_t variant, uint8_t light);

    [[nodiscard]] BlockType getType() const;

    [[nodiscard]] uint8_t getVariant() const;

    [[nodiscard]] uint8_t getLight() const;

    void setType(BlockType type_);

    void setType(uint32_t type_);

    void setVariant(uint8_t variant);

    void setLight(uint8_t light);

    [[nodiscard]] bool isSolid() const;

    [[nodiscard]] bool isVoid() const;
};

#endif //VOXEL_H
