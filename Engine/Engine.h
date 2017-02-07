#ifndef OA_ENGINE_H
#define OA_ENGINE_H

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include "taskmanager.h"
#include "log.h"
#include "eventchannel.h"
#include "config.h"
#include "video.h"

class Engine {
public:
	typedef boost::shared_ptr<System> SystemPtr;
	typedef std::map<std::string, SystemPtr> SystemMap;
	
	Engine();
	~Engine();
	
	void run(int argc, char* argv[]);
	void stop();
	
	void add(SystemPtr s);
	SystemPtr get(const std::string& systemName) const;
	
	const boost::shared_ptr<Config>& config();
	const boost::shared_ptr<Video>& video();
	
private:
	void initializeSystems();
	void shutdownSystems();
	
	boost::shared_ptr<Config> mConfig;
	boost::shared_ptr<Video> mVideo;
	
	SystemMap mSystemMap;
	TaskManager mTaskManager;
	EventChannel mChannel;
};

#endif