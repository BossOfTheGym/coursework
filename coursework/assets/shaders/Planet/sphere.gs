#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in vec4 tesModelPos[];
in vec4 tesWorldPos[];
in vec3 tesNormal[];
in vec3 tesColor[];

out vec4 geoModelPos;
out vec4 geoWorldPos;
out vec3 geoNormal;
out vec3 geoColor;
out vec3 geoGomo;


void main()
{
	gl_Position = gl_in[0].gl_Position;
	geoModelPos = tesModelPos[0];
	geoWorldPos = tesWorldPos[0];
	geoNormal   = tesNormal[0];
	geoColor    = tesColor[0];
	geoGomo     = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	geoModelPos = tesModelPos[1];
	geoWorldPos = tesWorldPos[1];
	geoNormal   = tesNormal[1];
	geoColor    = tesColor[1];
	geoGomo     = vec3(0.0f, 1.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	geoModelPos = tesModelPos[2];
	geoWorldPos = tesWorldPos[2];
	geoNormal   = tesNormal[2];
	geoColor    = tesColor[2];
	geoGomo     = vec3(0.0f, 0.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}