#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
out vec4 Colour;
void main() {
	gl_Position = vec4(aPos, 1.0f);
	Colour = vec4(aColour, 1.0f);
}