#include "IncludeOgl.h"
#include "GlfwFrame.h"

#include <functional>

ogl::GlfwFrame::GlfwFrame()
	: camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	// camera
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	firstMouse = true;

	// timing
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	// lighting info
	lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	create_window();
}

ogl::GlfwFrame::~GlfwFrame()
{
	/* glfw terminated, clearning all previously allocated GLFW resources. */

	glfwTerminate();
}

int ogl::GlfwFrame::main()
{
	build();

	while (!glfwWindowShouldClose(m_window))
	{
		before_draw();

		// input
		processInput(m_window);

		draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	release();

	return 0;
}

void ogl::GlfwFrame::create_window()
{
	/* glfw initialize and configue */

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	/* glfw window creation */

	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	assert(m_window != nullptr && "Failed to create window");

	if (!m_window)
		return;

	glfwMakeContextCurrent(m_window);

	/* glfw event callback */
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		if (auto pFrameWnd = static_cast<GlfwFrame*>(glfwGetWindowUserPointer(window)))
		{
			auto func = std::bind(&GlfwFrame::framebuffer_size_callback, pFrameWnd, ::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(window, width, height);
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow * window, double xpos, double ypos)
	{
		if (auto pFrameWnd = static_cast<GlfwFrame*>(glfwGetWindowUserPointer(window)))
		{
			auto func = std::bind(&GlfwFrame::mouse_callback, pFrameWnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(window, xpos, ypos);
		}
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow * window, double xoffset, double yoffset)
	{
		if (auto pFrameWnd = static_cast<GlfwFrame*>(glfwGetWindowUserPointer(window)))
		{
			auto func = std::bind(&GlfwFrame::scroll_callback, pFrameWnd, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			func(window, xoffset, yoffset);
		}
	});

	// tell GLFW to capture our mouse
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* glad load all OpenGL function pointers */

	auto res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(res && "Failed to initialize GLAD");
}

void ogl::GlfwFrame::before_draw()
{
	// per-frame time logic
	// --------------------
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ogl::GlfwFrame::processInput(GLFWwindow * window)
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
	{
		shadows = !shadows;
		shadowsKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		shadowsKeyPressed = false;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void ogl::GlfwFrame::framebuffer_size_callback(GLFWwindow *, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void ogl::GlfwFrame::mouse_callback(GLFWwindow *, double xpos, double ypos)
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

	camera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ogl::GlfwFrame::scroll_callback(GLFWwindow *, double, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int ogl::GlfwFrame::loadTexture(const char * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = image::GetInstance().load(path, width, height, nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		image::GetInstance().free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		image::GetInstance().free(data);
	}

	return textureID;
}
