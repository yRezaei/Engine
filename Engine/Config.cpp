#include "config.h"
#include <fstream>

namespace po = boost::program_options;

Config::Config():
	System("Config")
{
}

Config::~Config() {
}

bool Config::load(const std::string& filename) {
	std::ifstream fs(filename.c_str());
	
	if (!fs) {
		mLog << "Failed to open config file: " << filename;
		return false;
	}
	else {
		po::store(po::parse_config_file(fs, mOptions, true), mVariables);
		po::notify(mVariables);
		return true;
	}
}

void Config::parseCommandLine(int argc, char* argv[]) {
	po::store(po::parse_command_line(argc, argv, mOptions), mVariables);
		po::notify(mVariables);
}

po::options_description& Config::settings() {
	return mOptions;
}

bool Config::isSet(const std::string& name) const {
	return (mVariables.count(name) > 0);
}