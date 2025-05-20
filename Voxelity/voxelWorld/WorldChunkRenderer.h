//
// Created by adrian on 17/05/25.
//

#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include <memory>
#include <glm/glm.hpp>

#include "rendering/shader/Shader.h"
#include "math/Frustum.h"
#include "textures/Texture.h"
#include "WorldChunkData.h"
#include "rendering/graphics/Pointer.h"

class WorldChunkRenderer {
public:
    WorldChunkRenderer();

    void render(const WorldChunkData& chunkManager,
                const glm::vec3& cameraPosition,
                const glm::mat4& view,
                const glm::mat4& projection,
                const glm::vec3& lightDirection,
                const glm::vec3& lightColor,
                const glm::vec3& ambientColor);

private:
    std::unique_ptr<Shader> chunkShader;
    std::unique_ptr<Shader> pointerGlowShader;
    std::unique_ptr<Texture> textureAtlas;
    Frustum frustum;

    std::unique_ptr<Pointer> pointer;

    void prepareShader(const glm::vec3& cameraPosition,
                       const glm::mat4& view,
                       const glm::mat4& projection,
                       const glm::vec3& lightDir,
                       const glm::vec3& lightCol,
                       const glm::vec3& ambientCol) const;

    void prepareTextures() const;
};

#endif //CHUNKRENDERER_H
