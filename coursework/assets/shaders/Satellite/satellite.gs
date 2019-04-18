#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in dvec3 vNormal[];

flat out dvec3 geoNormal[3];
out vec3 geoGomo;


void main()
{
	geoNormal[0] = vNormal[0];
	geoNormal[1] = vNormal[1];
	geoNormal[2] = vNormal[2];

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
