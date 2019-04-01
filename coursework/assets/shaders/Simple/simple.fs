#version 430 core

in vec4 vColor;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vTex;


out vec4 fColor;


void main()
{
	fColor = vec4(1.0, 0.0, 0.0, 1.0);
}