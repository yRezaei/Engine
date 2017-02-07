#include "video.h"

Video::Video(): 
	System("Video"),
	mWindow(new Window())
{
	addSetting("Width", &mWindow->mWidth);
	addSetting("Height", &mWindow->mHeight);

	addSetting("RedBits", &mWindow->mRedBits);
	addSetting("GreenBits", &mWindow->mGreenBits);
	addSetting("BlueBits", &mWindow->mBlueBits);
	addSetting("AlphaBits", &mWindow->mAlphaBits);

	addSetting("DepthBits", &mWindow->mDepthBits);
	addSetting("StencilBits", &mWindow->mStencilBits);

	addSetting("FullScreen", &mWindow->mFullScreen);
	addSetting("Title", &mWindow->mTitle);

	enableUpdater(Task::SINGLETHREADED_REPEATING);
}

Video::~Video() {	
}

bool Video::init() {
	if (glfwInit() == GL_FALSE) {
		mLog << "Failed to initialize GLFW";
		return false;
	}
	
	int versionMajor, versionMinor, versionRevision;
	glfwGetVersion(&versionMajor, &versionMinor, &versionRevision);
	mLog << "Initialized GLFW " << versionMajor << "." << versionMinor << " rev. " << versionRevision;

	if (!mWindow->create()) {
		mLog << "Failed to create window";
		return false;
	}

	EventChannel chan;
	chan.broadcast(WindowCreated(mWindow));

	return true;
}

void Video::shutdown() {
	glfwTerminate();
}

void Video::update() {
	EventChannel chan;

	chan.broadcast(PreUpdate());
	glfwSwapBuffers(mWindow->window_ptr_);
	chan.broadcast(PostUpdate());
}