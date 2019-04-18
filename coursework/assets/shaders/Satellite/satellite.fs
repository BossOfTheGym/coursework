#version 430 core

flat in dvec3 geoNormal[3];
in vec3 geoGomo;


out vec4 fColor;


uniform dvec3 color;


void main()
{
	vec4 frag = vec4(vec3(color), 1.0);

	if(abs(geoGomo[0]) < 0.05 || abs(geoGomo[1]) < 0.05 || abs(geoGomo[2]) < 0.05)
	{
		frag.xyz = vec3(0.0);
	}

    fColor = frag;
}