#version 330 core
in vec2 TexCoords;
in float dist2mirror;

out vec4 color;

uniform sampler2D texture1;

void main() {
	color = dist2mirror <=0 ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : texture(texture1, TexCoords);
}