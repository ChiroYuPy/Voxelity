//
// Created by adrian on 15/05/25.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
public:
    explicit Image(const std::string& filepath, bool flipVertically = true);
    Image(const unsigned char* memory, int size);
    ~Image();

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }
    [[nodiscard]] int getChannels() const { return channels; }
    [[nodiscard]] const unsigned char* getData() const { return data; }
    unsigned char* getData() { return data; }
    [[nodiscard]] bool isValid() const { return data != nullptr; }

private:
    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;

    void loadFromFile(const std::string& filepath, bool flipVertically);
    void loadFromMemory(const unsigned char* memory, int size);
};

#endif //IMAGE_H
