#version 460 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uAtlas;

void main() {
    vec4 texColor = texture(uAtlas, vUV);
    FragColor = texColor;
}