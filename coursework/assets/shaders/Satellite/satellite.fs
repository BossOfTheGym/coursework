#version 430 core

in vec3 vNormal;
in vec3 geoGomo;


out vec4 fColor;


uniform vec3 color;


void main()
{
	vec4 frag = vec4(color, 1.0);

	if(abs(geoGomo[0]) < 0.05f || abs(geoGomo[1]) < 0.05f || abs(geoGomo[2]) < 0.05f)
	{
		frag.xyz = vec3(0.0);
	}

    fColor = frag;
}