#pragma once

#include "FrameWnd.h"

class CModelWnd : public frm::CFrameWnd
{
public:
	CModelWnd();
	virtual ~CModelWnd() = default;

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

private:
	// build and compile shaders
	// -------------------------
	Shader ourShader;// ("1.model_loading.vs", "1.model_loading.fs");

	// load models
	// -----------
	Model ourModel;// (FileSystem::getPath("resources/objects/backpack/backpack.obj"));

	// camera
	Camera camera;// (glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX;// = SCR_WIDTH / 2.0f;
	float lastY;// = SCR_HEIGHT / 2.0f;
	bool firstMouse;// = true;

	// timing
	float deltaTime;// = 0.0f;
	float lastFrame;// = 0.0f;
};

