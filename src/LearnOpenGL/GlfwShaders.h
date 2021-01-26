#pragma once

#include "GlfwFrame.h"

class GlfwShaders : public ogl::GlfwFrame
{
public:
	GlfwShaders();
	virtual ~GlfwShaders();

private:
	virtual void build();
	virtual void draw();
	virtual void release();

private:
	// build and compile our shader program
	// ------------------------------------
	Shader m_ourShader;// ("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

	unsigned int VBO, VAO;
};

