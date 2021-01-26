#pragma once

#include "GlfwFrame.h"

class GlfwLightingMaps : public ogl::GlfwFrame
{
public:
	GlfwLightingMaps();
	virtual ~GlfwLightingMaps();

private:
	virtual void build();
	virtual void draw();
	virtual void release();

private:
	// build and compile our shader zprogram
	// ------------------------------------
	Shader m_lightingShader;// ("4.2.lighting_maps.vs", "4.2.lighting_maps.fs");
	Shader m_lightCubeShader;//("4.2.light_cube.vs", "4.2.light_cube.fs");

	unsigned int VBO, cubeVAO;
	unsigned int lightCubeVAO;

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap;// = loadTexture("resources/textures/container2.png");
	unsigned int specularMap;// = loadTexture("resources/textures/container2_specular.png");

};

