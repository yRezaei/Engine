#include "system.h"

System::SystemInitializing::SystemInitializing(System* s):
	mSystem(s)
{
}

System::SystemShuttingDown::SystemShuttingDown(System* s):
	mSystem(s)
{
}

System::SystemUpdater::SystemUpdater(System* s, unsigned int taskFlags):
	mSystem(s),
	Task(taskFlags)
{
}

void System::SystemUpdater::run() {
	mSystem->update();
}

System::System(const std::string& name):
	mName(name)
{
}


System::~System() {
}

bool System::init() {
	mChannel.broadcast(SystemInitializing(this));
	
	mLog.setPrefix("[" + getName() + "] ");
	mLog.add(new std::ofstream(getName() + ".log"));
	
	return true;
}

void System::update() {
}

void System::shutdown() {
	mChannel.broadcast(SystemShuttingDown(this));
}

const std::string& System::getName() const {
	return mName;
}

void System::enableUpdater(unsigned int taskFlags) {
	mSystemUpdater.reset(new SystemUpdater(this, taskFlags));
}

