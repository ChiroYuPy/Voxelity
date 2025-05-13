#version 460 core
layout(location = 0) in ivec3 in_voxelPos;
layout(location = 1) in int in_faceId;
layout(location = 2) in vec3 in_color;

out vec3 vColor;

uniform mat4 uProjection;
uniform mat4 uView;

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

void main() {
    int vertexIdx = gl_VertexID % 6;
    vec3 offset = faceVertices[in_faceId][vertexIdx];
    vec3 worldPos = vec3(in_voxelPos) + offset;

    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
    vColor = in_color;
}