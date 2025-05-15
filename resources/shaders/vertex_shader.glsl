#version 460 core

layout(location = 0) in ivec3 in_voxelPos;
layout(location = 1) in int in_faceId;
layout(location = 2) in int in_blockId;

out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

uniform mat4 uProjection;
uniform mat4 uView;

// Normales fixes par face
const vec3 faceNormals[6] = vec3[](
vec3(1, 0, 0),  // +X
vec3(-1, 0, 0), // -X
vec3(0, 1, 0),  // +Y
vec3(0, -1, 0), // -Y
vec3(0, 0, 1),  // +Z
vec3(0, 0, -1)  // -Z
);

const vec3 faceVertices[6][6] = vec3[6][6](
vec3[6](vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5)),
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5)),
vec3[6](vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5)),
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5)),
vec3[6](vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5)),
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

    // Passer la position et normale au fragment shader
    vWorldPos = worldPos;
    vNormal = faceNormals[in_faceId];

    float texX = float((in_blockId - 1) % 2) * 0.5;
    float texY = float((in_blockId - 1) / 2) * 0.5;
    vUV = uvCoords[vertexIdx] * 0.5 + vec2(texX, texY);

    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
