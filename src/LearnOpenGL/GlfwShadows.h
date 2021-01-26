#pragma once

#include "GlfwFrame.h"

class GlfwShadows : public ogl::GlfwFrame
{
public:
	GlfwShadows();
	virtual ~GlfwShadows();

private:
	virtual void build();
	virtual void draw();
	virtual void release();

private:
	void renderScene(const Shader &shader);
	void renderCube();
	void renderQuad();

private:
	// build and compile shaders
	// -------------------------
	Shader m_shader;//("3.1.3.shadow_mapping.vs", "3.1.3.shadow_mapping.fs");
	Shader m_simpleDepthShader;//("3.1.3.shadow_mapping_depth.vs", "3.1.3.shadow_mapping_depth.fs");
	Shader m_debugDepthQuad;//("3.1.3.debug_quad.vs", "3.1.3.debug_quad_depth.fs");

	// load textures
	// -------------
	unsigned int m_woodTexture;// = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str());

	unsigned int planeVAO;
	unsigned int planeVBO;

	unsigned int depthMapFBO;
	unsigned int depthMap;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};

