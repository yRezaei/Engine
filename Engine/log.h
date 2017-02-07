#ifndef OA_CORE_LOG_H
#define OA_CORE_LOG_H

#include "libs.h"

class Logger {
private:
	class LogHelper;
	friend class LogHelper;

public:
	enum eErrorLevel {
		LOG_FATAL = 0x1 << 0,
		LOG_ERROR = 0x1 << 1,
		LOG_WARNING = 0x1 << 2,
		LOG_INFO = 0x1 << 3,
		LOG_DEBUG = 0x1 << 4,

		LOG_DEFAULT = LOG_FATAL | LOG_ERROR | LOG_WARNING | LOG_INFO,
		LOG_ALL = ~0x0
	};

	Logger(bool defaultOutput = true, bool addPrefix = true, bool addPostfix = true);
	virtual ~Logger();

	Logger& add(std::ostream* output, unsigned int messageFlags = LOG_DEFAULT);
	Logger& remove(std::ostream* output);
	Logger& setOutputFlags(std::ostream* output, unsigned int newFlags);
	unsigned int getNumOutputs() const;
		
	template <typename T>
	LogHelper operator << (const T& value) {
		if (mAddPrefix)
			mBuffer << mPrefix;
		
		mBuffer << value;

		return LogHelper(this);
	}

	LogHelper operator << (std::ostream& (*fn)(std::ostream& os));

	Logger& fatal();
	Logger& error();
	Logger& warning();
	Logger& info();
	Logger& debug();

	Logger& setFlags(unsigned int flags);
	unsigned int getFlags() const;

	void enablePrefix(bool enable = true) { mAddPrefix = enable; }
	void setPrefix(const std::string& prefix = "[Generic] ") { mPrefix = prefix; }
	void enablePostfix(bool enable = true) { mAddPostfix = enable; }
	void setPostfix(const std::string& postfix = "\n") { mPostfix = postfix; }
	
private:
	class LogHelper {
	public:
		LogHelper(Logger* parent);
		~LogHelper();

		template <typename T>
		LogHelper& operator << (const T& value) {
			mParent->mBuffer << value;
			return *this;
		}

		LogHelper& operator << (std::ostream& (*fn)(std::ostream& os));

	private:
		Logger* mParent;
	};

	void flush();
	void reset();

	typedef std::pair<unsigned int, std::ostream*> OutputPair;
	typedef std::vector<OutputPair> OutputList;
	typedef OutputList::iterator OutputIterator;

	std::stringstream mBuffer;

	bool mAddPrefix;
	bool mAddPostfix;
	std::string mPrefix;
	std::string mPostfix;

	OutputList mOutputs;
	unsigned int mFlags;
};

extern Logger gLog;

#endif
