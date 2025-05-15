//
// Created by adrian on 15/05/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <string>
#include <GLT.h>

class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    void bind(GLuint unit = 0) const;

    static void unbind() ;
    [[nodiscard]] GLuint getID() const { return textureID; }

private:
    GLuint textureID = 0;
    int width = 0;
    int height = 0;
};


#endif //TEXTURE_H
