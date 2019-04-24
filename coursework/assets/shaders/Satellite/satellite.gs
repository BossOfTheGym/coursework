#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in dvec3 vNormal[];

out vec3 geoNormal;
out vec3 geoGomo;


void main()
{

	gl_Position = gl_in[0].gl_Position;
	geoNormal   = vec3(vNormal[0]);
	geoGomo     = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	geoNormal   = vec3(vNormal[1]);
	geoGomo     = vec3(0.0f, 1.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	geoNormal   = vec3(vNormal[2]);
	geoGomo     = vec3(0.0f, 0.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}
