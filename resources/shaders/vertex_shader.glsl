#version 460 core

layout(location = 0) in uint in_data;

out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

uniform vec3 uChunkOffset;
uniform mat4 uProjection;
uniform mat4 uView;

// Normales fixes par face
const vec3 faceNormals[6] = vec3[](
vec3(1.0, 0.0, 0.0),  vec3(-1.0, 0.0, 0.0),
vec3(0.0, 1.0, 0.0),  vec3(0.0, -1.0, 0.0),
vec3(0.0, 0.0, 1.0),  vec3(0.0, 0.0, -1.0)
);

// Vertices par face
const vec3 faceVertices[6][6] = vec3[6][6](
vec3[6](vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5),
vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5)),
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), vec3(-0.5, 0.5, 0.5),
vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5)),
vec3[6](vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5),
vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5)),
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5),
vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5)),
vec3[6](vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5),
vec3(-0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5)),
vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5),
vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, -0.5, -0.5))
);

// Coordonnées UV
const vec2 uvCoords[6] = vec2[](
vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)
);

void main() {
    // Décompactage
    uint positionX = in_data         & 15u  ;
    uint positionY = (in_data >> 4)  & 15u  ;
    uint positionZ = (in_data >> 8)  & 15u  ;
    uint face      = (in_data >> 12) & 7u   ;
    uint type      = (in_data >> 15) & 255u ;

    int vertexIdx = gl_VertexID % 6;
    vec3 offset = faceVertices[int(face)][vertexIdx];

    // Conversion explicite uint → float
    vec3 worldPos = uChunkOffset + vec3(positionX, positionY, positionZ) + offset;

    vWorldPos = worldPos;
    vNormal = faceNormals[int(face)];

    // Texture atlas (2x2, chaque case fait 0.5x0.5)
    float ftype = float(type - 1u);
    float texX = mod(ftype, 2.0) * 0.5;
    float texY = floor(ftype / 2.0) * 0.5;
    vUV = uvCoords[vertexIdx] * 0.5 + vec2(texX, texY);

    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
