#include "IncludeOgl.h"
#include "GlfwHelloTriangle.h"

GlfwHelloTriangle::GlfwHelloTriangle()
{
}

GlfwHelloTriangle::~GlfwHelloTriangle()
{
}

void GlfwHelloTriangle::build()
{
}

void GlfwHelloTriangle::draw()
{
	glClearColor(0.5f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GlfwHelloTriangle::release()
{
}
