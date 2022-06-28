#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec2 texPos;

void main() {
	FragColor = texture(texture1, vec2((TexCoord.x + texPos.x) / 16, (TexCoord.y + texPos.y) / 16));
}