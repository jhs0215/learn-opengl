#include "includeOgl.h"
#include "FrameWnd.h"
#include "ProgramDirectory.h"

using namespace frm;

CFrameWnd::CFrameWnd()
{
	auto modulepath = CProgramDirectory::GetModuleDirectory();
	SetCurrentDirectory(modulepath.c_str());

	/* glfw initialize and configue */

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	/* glfw window creation */

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	assert(window != nullptr && "Failed to create window");

	if(window)
		glfwMakeContextCurrent(window);

	/* glad load all OpenGL function pointers */

	auto res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(res && "Failed to initialize GLAD");
}

CFrameWnd::~CFrameWnd()
{
	/* glfw terminated, clearning all previously allocated GLFW resources. */

	glfwTerminate();
}

void CFrameWnd::Run()
{
	if (window == nullptr)
		return;

	WindowProperty();

	InitRender();

	while (!glfwWindowShouldClose(window))
	{
		PreDraw();

		// input
		ProcessInput();

		Draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ReleaseRender();
}

void CFrameWnd::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}