#version 430 core

layout(location = 0) in dvec3 vertex;
layout(location = 2) in dvec3 normal;


out dvec3 vNormal;


uniform dmat4 model;
uniform dmat4 view;
uniform dmat4 proj;


void main()
{
	gl_Position = vec4(proj * view * model * dvec4(vertex, 1.0));

	vNormal = normal;
}