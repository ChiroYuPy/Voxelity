#version 460 core

layout(location = 0) in ivec3 in_voxelPos;
layout(location = 1) in int in_faceId;
layout(location = 2) in int in_blockId;

out vec2 vUV;

uniform mat4 uProjection;
uniform mat4 uView;
uniform vec3 uLightDirection; // Direction de la lumière

const vec3 faceVertices[6][6] = vec3[6][6](
// +X
vec3[6](vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5)),
// -X
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5)),
// +Y
vec3[6](vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5)),
// -Y
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5)),
// +Z
vec3[6](vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5)),
// -Z
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, -0.5, -0.5))
);

const vec2 uvCoords[6] = vec2[6](
vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)
);

void main() {
    int vertexIdx = gl_VertexID % 6;
    vec3 offset = faceVertices[in_faceId][vertexIdx];
    vec3 worldPos = vec3(in_voxelPos) + offset;

    // Calcul des coordonnées UV en fonction du blockId
    // Atlas 2x2 -> chaque texture occupe 0.5x0.5 dans l'atlas
    float texX = float((in_blockId - 1) % 2) * 0.5;  // 0 ou 0.5
    float texY = float((in_blockId - 1) / 2) * 0.5;  // 0 ou 0.5
    vUV = uvCoords[vertexIdx] * 0.5 + vec2(texX, texY);  // Ajuste la coordonnée UV

    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}