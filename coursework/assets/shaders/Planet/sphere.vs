#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;


out vec3 vsPosition;
out vec3 vsColor;
out vec2 vsTex;

void main()
{
	vsPosition = pos;
    vsColor    = color;
    vsTex      = tex;
}