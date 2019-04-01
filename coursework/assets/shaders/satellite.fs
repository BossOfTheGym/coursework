#version 430 core

in vec4 tesModelPos;
in vec4 tesWorldPos;
in vec3 tesNormal;
in vec3 tesColor;

out vec4 FragColor;

uniform vec3 color;


void main()
{
	//result
    FragColor = vec4(color, 1.0f);
}