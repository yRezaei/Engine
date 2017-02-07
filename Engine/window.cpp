#include "window.h"

Window::Window() {
	//default to 800x600x32, 24bits depth, 8 bits stencil, (windowed)
	mWidth = 800;
	mHeight = 600;
		
	mAlphaBits = 8;
	mRedBits = 8;
	mGreenBits = 8;
	mBlueBits = 8;

	mDepthBits = 24;
	mStencilBits = 8;

	mFullScreen = false;

	mTitle = "Overdrive Assault";
}

Window::~Window() {
}

bool Window::create() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ptr_ = glfwCreateWindow(
		mWidth,
		mHeight,
		mTitle.c_str(),
		nullptr,
		nullptr
	);

	if (!window_ptr_)
		return false;

	glfwMakeContextCurrent(window_ptr_);

		/*mRedBits,
		mGreenBits,
		mBlueBits,
		mAlphaBits,
		mDepthBits,
		mStencilBits,
		mFullScreen ? GLFW_FULLSCREEN : GLFW_WINDOW
	);*/

	glfwSwapInterval(0); //disable vertical sync
	return true;
}

void Window::destroy() {
	glfwDestroyWindow(window_ptr_);
}