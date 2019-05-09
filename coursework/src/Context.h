#pragma once

#include <Common.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>


class Context
{
private:
	Context();


public:
	static bool init(int width, int height, const String& name);

	static Context* getContext();

	static void terminate();


private:
	static Context* sContext;

	
public:
	GLFWwindow* mWindow;
};