#version 430 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in vec3 tex;

out vec4 vColor;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vTex;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	gl_Position = proj * view * model * vec4(vertex, 1.0);

	vColor = color;
	vNormal = normal;
	vTangent = tangent;
	vBitangent = bitangent;
	vTex = tex;
}