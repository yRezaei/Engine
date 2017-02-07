#include "log.h"

Logger::Logger(bool defaultOutput, bool addPrefix, bool addPostfix) {
	mFlags = LOG_DEFAULT;

	mAddPrefix = addPrefix;
	mAddPostfix = addPostfix;
	mPrefix = "[Generic] ";
	mPostfix = "\n";
	mBuffer.clear();

	if (defaultOutput)
		add(&std::cout);
}

Logger::~Logger() {
}

Logger::LogHelper Logger::operator << (std::ostream& (*fn)(std::ostream& os)) {
	std::ostringstream oss;
	fn(oss);
	return (LogHelper(this) << oss.str());
}

Logger::LogHelper::LogHelper(Logger* parent):
	mParent(parent)
{
}

Logger::LogHelper::~LogHelper() {
	if (mParent->mAddPostfix)
		mParent->mBuffer << mParent->mPostfix;
	mParent->flush();
}

Logger::LogHelper& Logger::LogHelper::operator << (std::ostream& (*fn)(std::ostream& os)) {
	std::ostringstream oss;
	fn(oss);
	return ((*this) << oss.str());
}

void Logger::flush() {
	for (OutputIterator it = mOutputs.begin(); it != mOutputs.end(); ++it)
		if (it->first & mFlags)
			(*(it->second)) << mBuffer.str();
	mBuffer.str("");
	reset();
}

Logger& Logger::add(std::ostream* output, unsigned int flags) {
	mOutputs.push_back(std::make_pair(flags, output));
	return *this;
}

Logger& Logger::remove(std::ostream* output) {
	for (OutputIterator it = mOutputs.begin(); it != mOutputs.end(); ++it)
		if (it->second == output) {
			mOutputs.erase(it);
			break;
		}
	return *this;
}
	
unsigned int Logger::getNumOutputs() const {
	return static_cast<unsigned int>(mOutputs.size());
}

Logger& Logger::setOutputFlags(std::ostream* output, unsigned int newFlags) {
	for (OutputIterator it = mOutputs.begin(); it != mOutputs.end(); ++it)
		if (it->second == output) {
			it->first = newFlags;
			break;
		}
	return *this;
}

Logger& Logger::fatal() {
	mFlags = LOG_FATAL;
	return *this;
}

Logger& Logger::error() {
	mFlags = LOG_ERROR;
	return *this;
}

Logger& Logger::warning() {
	mFlags = LOG_WARNING;
	return *this;
}

Logger& Logger::info() {
	mFlags = LOG_INFO;
	return *this;
}

Logger& Logger::debug() {
	mFlags = LOG_DEBUG;
	return *this;
}

Logger& Logger::setFlags(unsigned int flags) {
	mFlags = flags;
	return *this;
}

unsigned int Logger::getFlags() const {
	return mFlags;
}

void Logger::reset() {
	mFlags = LOG_DEFAULT;
}

Logger gLog(true);
