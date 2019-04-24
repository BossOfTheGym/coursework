#version 430 core

in vec3 geoNormal;
in vec3 geoGomo;


out vec4 fColor;


uniform dvec3 color;


void main()
{
	vec4 frag = vec4(vec3(color), 1.0);

	float d = min(min(geoGomo[0], geoGomo[1]), geoGomo[2]);
	if(d < 0.015)
	{
		frag.xyz = vec3(0.0);
	}

    fColor = frag;
}