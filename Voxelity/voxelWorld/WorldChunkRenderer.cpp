//
// Created by adrian on 17/05/25.
//

#include "WorldChunkRenderer.h"

#include <ranges>

#include "chunk/Chunk.h"
#include "core/Constants.h"
#include "core/utils/Profiler.h"
#include "textures/Texture.h"

WorldChunkRenderer::WorldChunkRenderer() {
    chunkShader = std::make_unique<Shader>(
        "../resources/shaders/vertex_shader.glsl",
        "../resources/shaders/fragment_shader.glsl");

    textureAtlas = std::make_unique<Texture>(
        "../resources/textures/atlas.png");

    pointer = std::make_unique<Pointer>();
}

void WorldChunkRenderer::render(const ChunkStorage& chunkManager,
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

    for (const auto &chunkPtr: chunkManager.chunks | std::views::values) {
        const auto& chunk = *chunkPtr;

        // frustum culling (do not render chunks outer of the view area)
        const glm::vec3 chunkWorldMin = chunk.getPosition() * Constants::CHUNK_SIZE;
        constexpr glm::vec3 halfSize = glm::vec3(Constants::CHUNK_SIZE) * 0.5f;

        if (const glm::vec3 center = chunkWorldMin + halfSize; frustum.intersectsAABB(center, halfSize)) {
            chunkShader->setUniform("uChunkOffset", chunk.getWorldPosition());
            chunk.render();
        }
    }

    pointer->render(view, projection);
}

void WorldChunkRenderer::prepareShader(const glm::vec3& cameraPosition,
                                  const glm::mat4& view,
                                  const glm::mat4& projection,
                                  const glm::vec3& lightDir,
                                  const glm::vec3& lightCol,
                                  const glm::vec3& ambientCol) const {
    // using this shader
    chunkShader->use();

    // Camera
    chunkShader->setUniform("uCameraPos", cameraPosition);
    chunkShader->setUniform("uView", view);
    chunkShader->setUniform("uProjection", projection);

    // Lighting
    chunkShader->setUniform("uLightDirection", glm::normalize(lightDir));
    chunkShader->setUniform("uLightColor", lightCol);
    chunkShader->setUniform("uAmbientColor", ambientCol);

    // Fog
    chunkShader->setUniform("uFogColor", glm::vec3(0.55f, 0.55f, 0.65f));
    chunkShader->setUniform("uFogStart", Constants::CHUNK_SIZE * Constants::RENDER_DISTANCE * 0.6f);
    chunkShader->setUniform("uFogEnd", Constants::CHUNK_SIZE * Constants::RENDER_DISTANCE * 1.f);
}

void WorldChunkRenderer::prepareTextures() const {
    glActiveTexture(GL_TEXTURE0);
    textureAtlas->bind(0);
    chunkShader->setUniform("uAtlas", 0);
}
