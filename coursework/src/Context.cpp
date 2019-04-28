#include "Context.h"


Context::Context()
	: mWindow(nullptr)
{}


//statics
Context* Context::context = nullptr;


int Context::init(int width, int height, const String& name)
{
	auto glfwErr = glfwInit();

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	Context::context = new Context();
	Context::context->window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(Context::context->window);

	auto glewErr = glewInit();

	//TODO
	return 0;
}

void Context::terminate()
{
	glfwDestroyWindow(Context::context->window);
	glfwTerminate();
}

Context* Context::getContext()
{
	return Context::context;
}