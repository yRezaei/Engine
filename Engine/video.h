#ifndef OA_VIDEO_H
#define OA_VIDEO_H

#include "System.h"
#include "Window.h"
#include "eventchannel.h"
#include <boost/shared_ptr.hpp>
#include <GLFW/glfw3.h>

class Video: public System {
public:
	Video();
	virtual ~Video();

	virtual bool init();
	virtual void update();
	virtual void shutdown();

	struct WindowCreated {
		WindowCreated(const boost::shared_ptr<Window>& window): mWindow(window) {}

		boost::shared_ptr<Window> mWindow;
	};

	struct PreUpdate {};
	struct PostUpdate {};

private:
	boost::shared_ptr<Window> mWindow;
};


#endif
