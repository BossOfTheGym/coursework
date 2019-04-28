#pragma once

#include <Common.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>


class Context
{
private:
	Context();


public:
	static int init();

	static Context* getContext() const;

	static void terminate();


private:
	static Context* context;

	
public:
	GLFWwindow* window;
};