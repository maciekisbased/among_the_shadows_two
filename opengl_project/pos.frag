#version 330 core

in vec3 vertPositions;
out vec4 fragColor;

void main()
{
	fragColor = vec4(vertPositions, 1.0);
}