#version 430 core

layout(location = 0) in vec3 vertex;
layout(location = 2) in vec3 normal;


out vec3 vNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	gl_Position = proj * view * model * vec4(vertex, 1.0);

	vNormal = normal;
}