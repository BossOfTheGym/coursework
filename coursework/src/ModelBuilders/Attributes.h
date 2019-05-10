#pragma once


enum class Attributes : GLuint
{
	INVALID = GLuint(-1)
	, VERTEX = GLuint(0)
	, COLOR = GLuint(1)
	, NORMAL = GLuint(2)
	, TANGENT = GLuint(3)
	, BITANGENT = GLuint(4)
	, TEXTURE = GLuint(5)
};