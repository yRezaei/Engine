#include "Engine.h"
#include "System.h"

class DummySystem: 
	public System 
{

public:
	DummySystem(): 
	  System("Dummy") 
	{
		mCounter = 0;
		enableUpdater(Task::REPEATING);
	}

	virtual void update() {
		if (mCounter++ > 50000)
			mEngine->stop();
	}

protected:
	int mCounter;
};

int main(int argc, char* argv[]) {
	Engine theEngine;
	theEngine.add(Engine::SystemPtr(new DummySystem()));

	theEngine.run(argc, argv);
	
	return 0;
}