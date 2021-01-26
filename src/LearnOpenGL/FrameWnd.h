#pragma once

struct GLFWwindow;

namespace frm
{
	class CFrameWnd
	{
	public:
		CFrameWnd();
		virtual ~CFrameWnd();

	public:
		void Run();

	protected:
		virtual void WindowProperty() {}
		virtual void InitRender() {}
		virtual void ReleaseRender() {}
		virtual void PreDraw() {}
		virtual void Draw() {}
		virtual void ProcessInput();

	protected:
		GLFWwindow* window = nullptr;
		const int SCR_WIDTH = 800;
		const int SCR_HEIGHT = 600;

	};
}