#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in dvec3 vsPosition[];
in dvec3 vsColor[];
in dvec2 vsTex[];


out vec4 geoModelPos;
out vec4 geoWorldPos;
out vec3 geoNormal;
out vec3 geoColor;
out vec3 geoGomo;


uniform dmat4 model;
uniform dmat4 view;
uniform dmat4 proj;
uniform dmat4 invTrans;



void main()
{
	dmat4 vp = proj * view;
	dvec4 modelPos;
	dvec4 worldPos;
	dvec3 normal;

	//[0]
	modelPos = dvec4(vsPosition[0], 1.0);
	worldPos = model * modelPos;
	normal   = normalize(invTrans * dvec4(modelPos.xyz, 0.0)).xyz;

	geoModelPos = vec4(modelPos);
	geoWorldPos = vec4(worldPos);
	geoNormal   = vec3(normal);
	geoColor    = vec3(vsColor[0]);
	geoGomo     = vec3(1.0, 0.0, 0.0);
	gl_Position = vec4(vp * worldPos);

	EmitVertex();


	//[1]
	modelPos = dvec4(vsPosition[1], 1.0);
	worldPos = model * modelPos;
	normal   = normalize(invTrans * dvec4(modelPos.xyz, 0.0)).xyz;

	geoModelPos = vec4(modelPos);
	geoWorldPos = vec4(worldPos);
	geoNormal   = vec3(normal);
	geoColor    = vec3(vsColor[1]);
	geoGomo     = vec3(0.0, 1.0, 0.0);
	gl_Position = vec4(vp * worldPos);

	EmitVertex();


	//[2]
	modelPos = dvec4(vsPosition[2], 1.0);
	worldPos = model * modelPos;
	normal   = normalize(invTrans * dvec4(modelPos.xyz, 0.0)).xyz;

	geoModelPos = vec4(modelPos);
	geoWorldPos = vec4(worldPos);
	geoNormal   = vec3(normal);
	geoColor    = vec3(vsColor[2]);
	geoGomo     = vec3(0.0, 0.0, 1.0);
	gl_Position = vec4(vp * worldPos);

	EmitVertex();


	EndPrimitive();
}