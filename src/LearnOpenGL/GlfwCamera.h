#pragma once

#include "GlfwFrame.h"

class GlfwCamera : public ogl::GlfwFrame
{
public:
	GlfwCamera();
	virtual ~GlfwCamera();

private:
	virtual void build();
	virtual void draw();
	virtual void release();

private:
	// build and compile our shader zprogram
	// ------------------------------------
	Shader m_ourShader;// ("7.4.camera.vs", "7.4.camera.fs");

	unsigned int VBO, VAO;

	// load and create a texture 
		// -------------------------
	unsigned int texture1, texture2;
};

