#version 460 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform sampler2D uAtlas;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform vec3 uFogColor;
uniform float uFogStart;
uniform float uFogEnd;
uniform vec3 uCameraPos;

void main() {
    vec3 normal = normalize(vNormal);

    vec4 texColor = texture(uAtlas, vUV);
    if (texColor.a < 0.1) discard;

    // --- AMBIANT ---
    vec3 ambient = uAmbientColor;

    // --- DIFFUSE DIRECTIONNELLE ---
    float diff = max(dot(normal, -normalize(uLightDirection)), 0.0);
    vec3 diffuse = uLightColor * diff;

    vec3 lighting = ambient + diffuse;

    vec3 finalColor = texColor.rgb * lighting;

    // --- FOG ---
    vec2 camXZ = uCameraPos.xz;
    vec2 fragXZ = vWorldPos.xz;
    float distance = length(fragXZ - camXZ);
    float fogFactor = clamp((distance - uFogStart) / (uFogEnd - uFogStart), 0.0, 1.0);

    vec3 colorWithFog = mix(finalColor, uFogColor, fogFactor);

    FragColor = vec4(finalColor, texColor.a);
}
