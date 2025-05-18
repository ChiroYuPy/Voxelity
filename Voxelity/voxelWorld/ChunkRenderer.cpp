//
// Created by adrian on 17/05/25.
//

#include "ChunkRenderer.h"

#include <iostream>
#include <ranges>

#include "chunk/Chunk.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "textures/Texture.h"

ChunkRenderer::ChunkRenderer() {
    chunkShader = std::make_unique<Shader>(
        "../resources/shaders/vertex_shader.glsl",
        "../resources/shaders/fragment_shader.glsl");

    textureAtlas = std::make_unique<Texture>(
        "../resources/textures/atlas.png");
    textureAtlas->bind(0); // bind once at init
}

void ChunkRenderer::render(const WorldChunkData& chunkManager,
                           const glm::vec3& cameraPosition,
                           const glm::mat4& view,
                           const glm::mat4& projection,
                           const glm::vec3& lightDirection,
                           const glm::vec3& lightColor,
                           const glm::vec3& ambientColor) {
    PROFILE_FUNCTION();
    prepareShader(cameraPosition, view, projection, lightDirection, lightColor, ambientColor);
    prepareTextures();

    const glm::mat4 viewProj = projection * view;
    frustum.update(viewProj);

    chunkShader->use();
    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) {
        const auto& chunk = *chunkPtr;

        if (!chunk.getMesh().hasVisibleFaces()) continue;

        const glm::vec3 chunkWorldMin = chunk.getPosition() * Constants::ChunkSize;
        constexpr glm::vec3 halfSize = glm::vec3(Constants::ChunkSize) * 0.5f;
        const glm::vec3 center = chunkWorldMin + halfSize;

        if (frustum.intersectsAABB(center, halfSize)) {
            chunkShader->setUniform("uChunkOffset", chunk.getWorldPosition());
            chunk.getMesh().render();
        }
    }
}

void ChunkRenderer::prepareShader(const glm::vec3& cameraPosition,
                                  const glm::mat4& view,
                                  const glm::mat4& projection,
                                  const glm::vec3& lightDir,
                                  const glm::vec3& lightCol,
                                  const glm::vec3& ambientCol) const {
    chunkShader->use();

    chunkShader->setUniform("uView", view);
    chunkShader->setUniform("uProjection", projection);
    chunkShader->setUniform("uLightDirection", glm::normalize(lightDir));
    chunkShader->setUniform("uLightColor", lightCol);
    chunkShader->setUniform("uAmbientColor", ambientCol);

    chunkShader->setUniform("uFogColor", glm::vec3(0.65f, 0.65f, 0.75f));
    chunkShader->setUniform("uFogStart", Constants::ChunkSize * Constants::RenderDistance * 0.4f);
    chunkShader->setUniform("uFogEnd", Constants::ChunkSize * Constants::RenderDistance * 1.6f);
    chunkShader->setUniform("uCameraPos", cameraPosition);
}

void ChunkRenderer::prepareTextures() const {
    glActiveTexture(GL_TEXTURE0);
    textureAtlas->bind(0);
    chunkShader->setUniform("uAtlas", 0);
}
