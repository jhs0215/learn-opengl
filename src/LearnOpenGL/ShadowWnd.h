#pragma once

#include "FrameWnd.h"

class ShadowWnd : public frm::CFrameWnd
{
public:
	ShadowWnd();
	virtual ~ShadowWnd() = default;

private:
	virtual void WindowProperty() override;
	virtual void InitRender() override;
	virtual void ReleaseRender() override;
	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void ProcessInput() override;

private:
	void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height);
	void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos);
	void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset);
	unsigned int loadTexture(const char *path);
	
	void renderScene(const Shader &shader);
	void renderCube();
	void renderQuad();

private:	
	// camera
	Camera camera;// (glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX;// = SCR_WIDTH / 2.0f;
	float lastY;// = SCR_HEIGHT / 2.0f;
	bool firstMouse;// = true;

	// timing
	float deltaTime;// = 0.0f;
	float lastFrame;// = 0.0f;


	// build and compile shaders
	// -------------------------
	Shader simpleDepthShader;// ("3.1.1.shadow_mapping_depth.vs", "3.1.1.shadow_mapping_depth.fs");
	Shader debugDepthQuad;//("3.1.1.debug_quad.vs", "3.1.1.debug_quad_depth.fs");
	// meshes
	unsigned int planeVAO;
	unsigned int planeVBO;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	unsigned int woodTexture;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	// lighting info
	// -------------
	glm::vec3 lightPos;// (-2.0f, 4.0f, -1.0f);
};

