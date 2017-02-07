#ifndef OA_CONFIG_H
#define OA_CONFIG_H

#include <boost/program_options.hpp>
#include "system.h"

class Config: public System {
public:
	Config();
	virtual ~Config();
	
	bool load(const std::string& filename);
	void parseCommandLine(int argc, char* argv[]);
	
	boost::program_options::options_description& settings();
	
	bool isSet(const std::string& settingName) const;
		
	template <typename T>
	T get(const std::string& settingName) const;
		
private:
	boost::program_options::options_description mOptions;
	boost::program_options::variables_map mVariables;
};

template <typename T>
T Config::get(const std::string& settingName) const {
	if (isSet(settingName))
		return mVariables[name.c_str()].as<T>();
	else {
		mLog << "Failed to find variable: " << settingName;
		return T();
	}
}

#endif