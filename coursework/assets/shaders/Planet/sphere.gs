#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in dvec4 tesModelPos[];
in dvec4 tesWorldPos[];
in dvec3 tesNormal[];
in dvec3 tesColor[];

flat out dvec4 geoModelPos[3];
flat out dvec4 geoWorldPos[3];
flat out dvec3 geoNormal[3];
flat out dvec3 geoColor[3];
out vec3 geoGomo;






void main()
{
	geoModelPos[0] = tesModelPos[0];
	geoModelPos[1] = tesModelPos[1];
	geoModelPos[2] = tesModelPos[2];

	geoWorldPos[0] = tesWorldPos[0];
	geoWorldPos[1] = tesWorldPos[1];
	geoWorldPos[2] = tesWorldPos[2];

	geoNormal[0]   = tesNormal[0];
	geoNormal[1]   = tesNormal[1];
	geoNormal[2]   = tesNormal[2];

	geoColor[0]    = tesColor[0];
	geoColor[1]    = tesColor[1];
	geoColor[2]    = tesColor[2];


	gl_Position = gl_in[0].gl_Position;
	geoGomo     = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	geoGomo     = vec3(0.0f, 1.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	geoGomo     = vec3(0.0f, 0.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}