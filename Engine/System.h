#ifndef OA_SYSTEM_H
#define OA_SYSTEM_H

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include "log.h"
#include "eventchannel.h"
#include "task.h"

class Engine;

class System {
public:
	struct SystemInitializing {
		SystemInitializing(System* s);
		System* mSystem;
	};
	
	struct SystemShuttingDown {
		SystemShuttingDown(System* s);
		System* mSystem;
	};
	
	struct SystemUpdater: public Task {
		SystemUpdater(System* s, unsigned int taskFlags = Task::SINGLETHREADED_REPEATING);
		virtual void run();
		
		System* mSystem;
	};
	
	System(const std::string& name);
	virtual ~System();
	
	virtual bool init();
	virtual void update();
	virtual void shutdown();	
	
	const std::string& getName() const;
	
protected:
	void enableUpdater(unsigned int taskFlags = Task::SINGLETHREADED_REPEATING);
	
	template <typename T>
	void addSetting(const std::string& name_in_configFile, T* the_variable);
	
	std::string mName;
	EventChannel mChannel;
	Logger mLog;
	boost::shared_ptr<SystemUpdater> mSystemUpdater;
	boost::program_options::options_description mSettings;

	friend class Engine; //allows the Engine class to combine all system settings
	Engine* mEngine;
};

template <typename T>
void System::addSetting(const std::string& nameInConfigFile, T* var) {
	std::stringstream sstr;
	sstr << getName() << "." << nameInConfigFile;
	
	mSettings.add_options()
		(sstr.str().c_str(), boost::program_options::value<T>(var))
	;
}

#endif