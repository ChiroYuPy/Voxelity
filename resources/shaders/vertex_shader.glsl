#version 460 core

layout(location = 0) in ivec3 in_voxelPos;
layout(location = 1) in int in_faceId;
layout(location = 2) in int in_blockId;

out vec3 vColor;

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

vec3 hash31(int id) {
    vec3 p = vec3(float(id + 1)) * vec3(0.1031, 0.11369, 0.13787);
    p = fract(p);
    p += dot(p, p.yzx + 19.19);
    return fract(vec3((p.x + p.y) * p.z, (p.x + p.z) * p.y, (p.y + p.z) * p.x));
}

void main() {
    int vertexIdx = gl_VertexID % 6;
    vec3 offset = faceVertices[in_faceId][vertexIdx];
    vec3 worldPos = vec3(in_voxelPos) + offset;

    // Calcul de la normale de la face en fonction de in_faceId
    vec3 normal;
    if (in_faceId == 0) normal = vec3(1.0, 0.0, 0.0);  // +X
    else if (in_faceId == 1) normal = vec3(-1.0, 0.0, 0.0); // -X
    else if (in_faceId == 2) normal = vec3(0.0, 1.0, 0.0);  // +Y
    else if (in_faceId == 3) normal = vec3(0.0, -1.0, 0.0); // -Y
    else if (in_faceId == 4) normal = vec3(0.0, 0.0, 1.0);  // +Z
    else if (in_faceId == 5) normal = vec3(0.0, 0.0, -1.0); // -Z

    // Calcul de l'ombrage (Lambertian)
    vec3 lightDir = normalize(uLightDirection); // Normaliser la direction de la lumière
    float lightIntensity = max(dot(normal, lightDir), 0.0);

    // Ajout d'une lumière ambiante pour éclairer tous les voxels
    float ambientLight = 0.5;  // Valeur d'éclairage ambiant
    lightIntensity += ambientLight;

    // Appliquer la lumière à la couleur du voxel
    vec3 baseColor = hash31(in_blockId);  // Couleur de base du voxel
    vColor = baseColor * lightIntensity;   // Couleur finale modifiée par l'intensité de la lumière

    // Calcul de la position finale de chaque sommet dans l'espace de projection
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
