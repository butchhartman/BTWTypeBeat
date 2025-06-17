#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D aTexture;

void main() {
    FragColor = texture(aTexture, texCoord); 
}