#version 430 core

#define DRAW_FILLED 0
#define DRAW_EDGES  1

uniform int mode;

in  vec3 outColor;
out vec4 FragColor;


void main()
{
	if(mode == DRAW_FILLED)
	{
		FragColor = vec4(outColor, 1.0f);
	}
	else
	{
		FragColor = vec4(vec3(0.0f), 1.0f);
	}
}