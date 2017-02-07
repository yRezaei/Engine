#include "engine.h"

Engine::Engine():
	mConfig(new Config()),
	mVideo(new Video())
{
	add(mVideo); //system needs to be added to the task queues
}

Engine::~Engine() {
}

void Engine::run(int argc, char* argv[]) {
	for (SystemMap::iterator it = mSystemMap.begin(); it != mSystemMap.end(); ++it)
		mConfig->settings().add(it->second->mSettings);
		
	std::cout << "Loading assault.cfg... ";
	mConfig->load("assault.cfg");
	std::cout << "done." << std::endl;
		
	mConfig->parseCommandLine(argc, argv);
		
	initializeSystems();
	mTaskManager.start();
	shutdownSystems();
}

void Engine::stop() {
	mChannel.broadcast(TaskManager::StopEvent());
}

void Engine::add(SystemPtr s) {
	if (mSystemMap.find(s->getName()) == mSystemMap.end()) {
		if (s->mSystemUpdater.get() != NULL)
			mTaskManager.add(s->mSystemUpdater);
		mSystemMap.insert(std::make_pair(s->getName(), s));
		s->mEngine = this;
	}
	else
		gLog << "Engine::add - System already added: " << s->getName();
}

Engine::SystemPtr Engine::get(const std::string& systemName) const {
	SystemMap::const_iterator it = mSystemMap.find(systemName);
		
	if (it == mSystemMap.end()) {
		gLog << "Engine::get - cannot find system: " << systemName;
		return SystemPtr();
	}
	return it->second;
}

void Engine::initializeSystems() {
	for (SystemMap::iterator it = mSystemMap.begin(); it != mSystemMap.end(); ++it) {
		gLog.info() << "Initializing " << it->second->getName();
		it->second->init();
	}
}

void Engine::shutdownSystems() {
	//in reverse order
	for (SystemMap::reverse_iterator it = mSystemMap.rbegin(); it != mSystemMap.rend(); ++it) {
		gLog.info() << "Shutting down " << it->second->getName();
		it->second->shutdown();
	}
}

const boost::shared_ptr<Config>& Engine::config() {
	return mConfig;
}

const boost::shared_ptr<Video>& Engine::video() {
	return mVideo;
}