#pragma once

#include "GlfwFrame.h"

class GlfwHelloTriangle : public ogl::GlfwFrame
{
public:
	GlfwHelloTriangle();
	virtual ~GlfwHelloTriangle();

private:
	virtual void build();
	virtual void draw();
	virtual void release();
};

