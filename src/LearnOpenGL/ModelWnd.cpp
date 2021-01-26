#include "includeOgl.h"
#include "ModelWnd.h"

#include <functional>

CModelWnd::CModelWnd()
	: ourShader("Resource/Shader/1/model_loading.vs.glsl", "Resource/Shader/1/model_loading.fs.glsl")
	, ourModel("Resource/Object/backpack/backpack.obj")
	, camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	firstMouse = true;

	// timing
	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

void CModelWnd::WindowProperty()
{
	assert(window != nullptr && "not defined window");

	glfwSetWindowUserPointer(window, static_cast<void*>(this));
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* pWnd, int width, int height)
	{
		if (auto pFrameWnd = static_cast<CModelWnd*>(glfwGetWindowUserPointer(pWnd)))
		{
			auto func = std::bind(&CModelWnd::framebuffer_size_callback, pFrameWnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(pWnd, width, height);
		}
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow * pWnd, double xpos, double ypos)
	{
		if (auto pFrameWnd = static_cast<CModelWnd*>(glfwGetWindowUserPointer(pWnd)))
		{
			auto func = std::bind(&CModelWnd::mouse_callback, pFrameWnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(pWnd, xpos, ypos);
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow * pWnd, double xoffset, double yoffset)
	{
		if (auto pFrameWnd = static_cast<CModelWnd*>(glfwGetWindowUserPointer(pWnd)))
		{
			auto func = std::bind(&CModelWnd::scroll_callback, pFrameWnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(pWnd, xoffset, yoffset);
		}
	});

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CModelWnd::InitRender()
{
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
}

void CModelWnd::ReleaseRender()
{
}

void CModelWnd::PreDraw()
{
	// per-frame time logic
	// --------------------
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void CModelWnd::Draw()
{
	// render
		// ------
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	ourShader.use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	ourShader.setMat4("model", model);
	ourModel.Draw(ourShader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void CModelWnd::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CModelWnd::framebuffer_size_callback(GLFWwindow * /*pWnd*/, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void CModelWnd::mouse_callback(GLFWwindow * /*pWnd*/, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void CModelWnd::scroll_callback(GLFWwindow * /*pWnd*/, double /*xoffset*/, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
