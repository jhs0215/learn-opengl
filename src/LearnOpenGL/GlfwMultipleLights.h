#pragma once

#include "GlfwFrame.h"

class GlfwMultipleLights : public ogl::GlfwFrame
{
	// positions all containers
	static glm::vec3 cubePositions[]; 
	// positions of the point lights
	static glm::vec3 pointLightPositions[]; 

public:
	GlfwMultipleLights();
	virtual ~GlfwMultipleLights();

private:
	virtual void build();
	virtual void draw();
	virtual void release();

private:
	// build and compile our shader zprogram
	// ------------------------------------
	Shader m_lightingShader;// ("6.multiple_lights.vs", "6.multiple_lights.fs");
	Shader m_lightCubeShader;//("6.light_cube.vs", "6.light_cube.fs");

	unsigned int VBO, cubeVAO;
	unsigned int lightCubeVAO;

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap;// = loadTexture("resources/textures/container2.png");
	unsigned int specularMap;// = loadTexture("resources/textures/container2_specular.png");

};

