#version 430 core

layout(location = 0) in dvec3 vertex;
layout(location = 1) in dvec4 color;
layout(location = 2) in dvec3 normal;
layout(location = 3) in dvec3 tangent;
layout(location = 4) in dvec3 bitangent;
layout(location = 5) in dvec3 tex;

out vec4 vColor;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vTex;


uniform dmat4 model;
uniform dmat4 view;
uniform dmat4 proj;


void main()
{
	gl_Position = vec4(proj * view * model * dvec4(vertex, 1.0));

	vColor = vec4(color);
	vNormal = vec3(normal);
	vTangent = vec3(tangent);
	vBitangent = vec3(bitangent);
	vTex = vec3(tex);
}