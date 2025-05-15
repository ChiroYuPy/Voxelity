//
// Created by adrian on 15/05/25.
//

#include "textures/Image.h"

#include <stdexcept>

#include <IMG.h>

void Image::loadFromFile(const std::string& filepath, const bool flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically);
    data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    channels = 4;
    if (!data)
        throw std::runtime_error("Failed to load image from file: " + filepath);
}

void Image::loadFromMemory(const unsigned char* memory, const int size) {
    data = stbi_load_from_memory(memory, size, &width, &height, &channels, STBI_rgb_alpha);
    channels = 4;
    if (!data)
        throw std::runtime_error("Failed to load image from memory");
}

Image::Image(const std::string& filepath, const bool flipVertically) {
    loadFromFile(filepath, flipVertically);
}

Image::Image(const unsigned char* memory, const int size) {
    loadFromMemory(memory, size);
}

Image::~Image() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}