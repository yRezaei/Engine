#ifndef OA_WINDOW_H
#define OA_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>

class Window {
private:
	struct Error_callback
	{
		void operator()(int error, const char* description) {
			fprintf(stderr, "Error: %s\n", description);
		}
	};
	Error_callback error_callback;


public:
	friend class Video;

	Window();
	virtual ~Window();

	bool create();
	void destroy();

protected:
	GLFWwindow* window_ptr_;
	unsigned int mWidth;
	unsigned int mHeight;

	bool mFullScreen;

	unsigned int mRedBits;
	unsigned int mGreenBits;
	unsigned int mBlueBits;
	unsigned int mAlphaBits;

	unsigned int mDepthBits;
	unsigned int mStencilBits;

	std::string mTitle;

};

#endif
