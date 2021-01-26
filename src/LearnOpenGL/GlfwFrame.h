#pragma once

#include "learnopengl/shader.h"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"

struct GLFWwindow;

namespace ogl
{
	class GlfwFrame
	{
	public:
		GlfwFrame();
		virtual ~GlfwFrame();

	public:
		int main();

		// constructor
	protected:
		virtual void create_window();		
		virtual void build() {}
		virtual void before_draw();
		virtual void draw() {}
		virtual void release() {}

		// callback event
	protected:
		virtual void processInput(GLFWwindow *window);
		virtual void framebuffer_size_callback(GLFWwindow* /*window*/, int /*width*/, int /*height*/);
		virtual void mouse_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/);
		virtual void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/);

	protected:
		unsigned int loadTexture(const char *path);

	protected:
		GLFWwindow* m_window = nullptr;
		const int SCR_WIDTH = 800;
		const int SCR_HEIGHT = 600;

		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		// camera
		Camera camera;// (glm::vec3(0.0f, 0.0f, 3.0f));
		float lastX;// = SCR_WIDTH / 2.0f;
		float lastY;// = SCR_HEIGHT / 2.0f;
		bool firstMouse;// = true;

		// timing
		float deltaTime;// = 0.0f;
		float lastFrame;// = 0.0f;

		// lighting info
		glm::vec3 lightPos;// (0.0f, 0.0f, 0.0f);

		bool shadows = true;
		bool shadowsKeyPressed = false;
	};
}

