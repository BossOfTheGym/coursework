#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 vsPosition;
out vec3 vsColor;


void main()
{
	vsPosition = pos;
    vsColor    = color;
}