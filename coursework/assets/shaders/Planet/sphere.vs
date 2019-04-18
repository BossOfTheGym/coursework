#version 430 core

layout(location = 0) in dvec3 pos;
layout(location = 1) in dvec3 color;
layout(location = 2) in dvec2 tex;


out dvec3 vsPosition;
out dvec3 vsColor;
out dvec2 vsTex;

void main()
{
	vsPosition = pos;
    vsColor    = color;
    vsTex      = tex;
}