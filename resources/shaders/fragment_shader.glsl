#version 460 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform sampler2D uAtlas;

uniform vec3 uLightDirection;  // Direction lumière principale (normalisée)
uniform vec3 uLightColor;      // Couleur lumière principale (ex: vec3(1.0, 1.0, 0.9))
uniform vec3 uAmbientColor;    // Couleur lumière ambiante (ex: vec3(0.3, 0.3, 0.4))

// Lumière ponctuelle simple (exemple torche)
uniform vec3 uPointLightPos;
uniform vec3 uPointLightColor;
uniform float uPointLightRadius;

void main() {
    vec3 normal = normalize(vNormal);

    // Texture
    vec4 texColor = texture(uAtlas, vUV);
    if (texColor.a < 0.1) discard; // alpha test

    // --- AMBIANT ---
    vec3 ambient = uAmbientColor;

    // --- DIFFUSE DIRECTIONNELLE (SOLEIL) ---
    float diff = max(dot(normal, -normalize(uLightDirection)), 0.0);
    vec3 diffuse = uLightColor * diff;

    // --- LUMIÈRE PONCTUELLE SIMPLE ---
    vec3 lightDir = uPointLightPos - vWorldPos;
    float dist = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = clamp(1.0 - dist / uPointLightRadius, 0.0, 1.0);
    float diffPoint = max(dot(normal, lightDir), 0.0);
    vec3 pointLight = uPointLightColor * diffPoint * attenuation;

    // Combinaison
    vec3 lighting = ambient + diffuse + pointLight;

    // Appliquer l’éclairage à la couleur de la texture
    vec3 finalColor = texColor.rgb * lighting;

    FragColor = vec4(finalColor, texColor.a);
}
